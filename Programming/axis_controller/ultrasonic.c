
#include "project-defs.h"

// this variable controls whether ultrasonic will be constantly working or not
static bool ultrasonic_cycle_on = false;
                                         
static ULTRASONIC_STATUS ultrasonic_status = ULTRASONIC_IDLE;

static volatile ULTRASONIC_PHASE ultrasonic_current_phase = ULTRASONIC_SEND_TRIGGER_PHASE;

static volatile uint32_t ultrasonic_timer_echo_ticks = 0;

static GpioConfig trigger_pin = GPIO_PIN_CONFIG(ULTRASONIC_TRIGGER_PORT, ULTRASONIC_TRIGGER_PIN, GPIO_BIDIRECTIONAL_MODE); 

// enabling interrupt with rising edge detection first
// echo_pin.interruptTrigger = GPIO_FALLING_EDGE;  // change it to detecting rising edge
static GpioConfig echo_pin = { .port = ULTRASONIC_ECHO_PORT, .pin = ULTRASONIC_ECHO_PIN, .count = 1, .pinMode = GPIO_BIDIRECTIONAL_MODE,	.pinInterrupt = ENABLE_GPIO_PIN_INTERRUPT, .interruptTrigger = GPIO_RISING_EDGE, .wakeUpInterrupt = DISABLE_GPIO_PIN_WAKE_UP, \
	DEFAULTS_PU_NCS  DEFAULTS_SR_DR_IE  };

void ultrasonic_init(void) {

  // Setting trigger pin GpioConfig
  gpioConfigure(&trigger_pin);
  gpioWrite(&trigger_pin, 0);

  // Setting echo pin GpioConfig
  gpioConfigure(&echo_pin);

}

void ultrasonic_start_sequence(void) {

  // Process ultrasonic status state machine
  switch (ultrasonic_status) {

    case ULTRASONIC_IDLE:
      ultrasonic_status = ULTRASONIC_FIRST_RUN;
      break;

    case ULTRASONIC_ACTIVE:
    case ULTRASONIC_UNRESPONSIVE:
      // restarts ultrasonic_current_phase whatever it was before
      ultrasonic_current_phase = ULTRASONIC_SEND_TRIGGER_PHASE;
      break;

    case ULTRASONIC_FIRST_RUN:
      while(1) { printf("WTF?!?!, ultrasonic_status is first run?!?!?\n"); }
  }

}

void ultrasonic_stop_sequence(void) {

  ultrasonic_status = ULTRASONIC_IDLE;  // phase processing is halted
                                        //
  ultrasonic_current_phase = ULTRASONIC_SEND_TRIGGER_PHASE; // first phase, no interrupts is triggered
  
  stopTimer(ULTRASONIC_TIMER);  // stopping it just for good measure

}

void static ultrasonic_send_trigger(void) {

  gpioWrite(&trigger_pin, 1);

  startTimer(
      ULTRASONIC_TIMER, 
      frequencyToSysclkDivisor(PERIOD_us_TO_FREQ_INT(ULTRASONIC_TRIGGER_ON_TIME_us)),
      DISABLE_OUTPUT, 
      ENABLE_INTERRUPT, 
      FREE_RUNNING
      );

  ultrasonic_current_phase = ULTRASONIC_AWAIT_TRIGGER_PHASE;

}

void static ultrasonic_await_echo_rise(void) {

  // Time-out timer if no echo rise detected
  startTimer(
      ULTRASONIC_TIMER, 
      frequencyToSysclkDivisor(PERIOD_us_TO_FREQ_INT(ECHO_RISE_TIMEOUT_us)),
      DISABLE_OUTPUT, 
      ENABLE_INTERRUPT, 
      FREE_RUNNING
      );

  // changing to interrupt on rising edge if it was changed by a previous phase
  echo_pin.interruptTrigger = GPIO_RISING_EDGE; 
  gpioConfigure(&echo_pin);

  ultrasonic_current_phase = ULTRASONIC_AWAIT_ECHO_RISE_PHASE;

}

void static ultrasonic_await_echo_fall(void) {

  // Time-out timer if no echo rise detected
  startTimer(
      ULTRASONIC_TIMER, 
      frequencyToSysclkDivisor(PERIOD_us_TO_FREQ_INT(ECHO_FALL_TIMEOUT_us)),
      DISABLE_OUTPUT, 
      ENABLE_INTERRUPT, 
      FREE_RUNNING
      );

  // changing to interrupt on falling edge if it was changed by a previous phase
  echo_pin.interruptTrigger = GPIO_FALLING_EDGE; 
  gpioConfigure(&echo_pin);

  ultrasonic_current_phase = ULTRASONIC_AWAIT_ECHO_FALL_PHASE;

}


void processs_ultrasonic_phases(void) {

  if (ultrasonic_status != ULTRASONIC_IDLE) {

    switch (ultrasonic_current_phase) {

      case ULTRASONIC_SEND_TRIGGER_PHASE:
        ultrasonic_send_trigger();
        break;

      case ULTRASONIC_TRIGGER_SENT_PHASE:
        ultrasonic_await_echo_rise();
        break;

      case ULTRASONIC_ECHO_RISE_CAPTURED_PHASE:
        ultrasonic_await_echo_fall();
        break;

      case ULTRASONIC_ECHO_FALL_CAPTURED_PHASE:
      case ULTRASONIC_ECHO_TIMEOUT_PHASE:
        ultrasonic_start_sequence();
        break;
    }
  }
}

ULTRASONIC_STATUS ultrasonic_get_distance(uint16_t* distance) {

    if (ultrasonic_status == ULTRASONIC_ACTIVE) {

      *distance = ultrasonic_timer_echo_ticks*ULTRASONIC_COUNTER_TO_CM;

    }

    return ultrasonic_status;

}

INTERRUPT(ULTRASONIC_TIMER_ISR, ULTRASONIC_TIMER_INTERRUPT) {

  switch (ultrasonic_current_phase) {

    case ULTRASONIC_AWAIT_TRIGGER_PHASE:
      gpioWrite(&trigger_pin, 0);
      ultrasonic_current_phase = ULTRASONIC_TRIGGER_SENT_PHASE;
      break;

    case ULTRASONIC_AWAIT_ECHO_RISE_PHASE:
      // time out error, ULTRASONIC_UNRESPONSIVE !!
      ultrasonic_status = ULTRASONIC_UNRESPONSIVE;  // update state
      ultrasonic_current_phase = ULTRASONIC_ECHO_TIMEOUT_PHASE;  // restarting cycle
      break;

    case ULTRASONIC_AWAIT_ECHO_FALL_PHASE:
      // time out error, ULTRASONIC_UNRESPONSIVE !!
      ultrasonic_status = ULTRASONIC_UNRESPONSIVE;  // update state
      ultrasonic_current_phase = ULTRASONIC_ECHO_TIMEOUT_PHASE;  // restarting cycle
      break;

  }

}

INTERRUPT(ULTRASONIC_INT_PIN_ISR, ULTRASONIC_INT_PIN_INTERRUPT) {

  switch (ultrasonic_current_phase) {

    case ULTRASONIC_AWAIT_ECHO_RISE_PHASE:
      ultrasonic_current_phase = ULTRASONIC_ECHO_RISE_CAPTURED_PHASE;
      break;

    case ULTRASONIC_AWAIT_ECHO_FALL_PHASE:
      ultrasonic_timer_echo_ticks = stopTimer(ULTRASONIC_TIMER); // gotcha bitch :D
      ultrasonic_current_phase = ULTRASONIC_ECHO_FALL_CAPTURED_PHASE;
      ultrasonic_status = ULTRASONIC_ACTIVE;
      break;

  }

}
