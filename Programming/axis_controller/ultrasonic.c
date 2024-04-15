
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

}

void ultrasonic_stop_sequence(void) {

}

void static ultrasonic_send_trigger(void) {

}

void static ultrasonic_await_echo_rise(void) {

}

void static ultrasonic_await_echo_fall(void) {

}


void processs_ultrasonic_phases(void) {

  if (ultrasonic_cycle_on) {

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
        ultrasonic_start_sequence();
        break;
    }
  }
}

ULTRASONIC_STATUS ultrasonic_get_distance(uint16_t* distance)
{

  if (ultrasonic_cycle_on) {

    // if it's ULTRASONIC_IDLE, then it means it's probably the first cycle so still idle (i think)
    if (ultrasonic_status == ULTRASONIC_ACTIVE) {

      *distance = ultrasonic_timer_echo_ticks*ULTRASONIC_COUNTER_TO_CM;

    }

    return ultrasonic_status;

  } else {

    // no value distance value set if ultrasonic_cycle is not turned on, in other words can't have a one time measurement, it's either always measuring and the user can call the latest distance measured or no value will be set
    return ULTRASONIC_IDLE;

  }

}

INTERRUPT(ULTRASONIC_TIMER_ISR, ULTRASONIC_TIMER_INTERRUPT) {

  switch (ultrasonic_current_phase) {

    case ULTRASONIC_AWAIT_TRIGGER_PHASE:
      ultrasonic_current_phase = ULTRASONIC_TRIGGER_SENT_PHASE;
      // is there sth else to do here??
      break;

    case ULTRASONIC_AWAIT_ECHO_RISE_PHASE:
      // time out error, ULTRASONIC_UNRESPONSIVE !!
      break;

    case ULTRASONIC_AWAIT_ECHO_FALL_PHASE
      // time out error, ULTRASONIC_UNRESPONSIVE !!
      break;

  }

}

INTERRUPT(ULTRASONIC_INT_PIN_ISR, ULTRASONIC_INT_PIN_INTERRUPT) {

  switch (ultrasonic_current_phase) {

    case ULTRASONIC_AWAIT_ECHO_RISE_PHASE:
      //TODO:
      break;

    case ULTRASONIC_AWAIT_ECHO_FALL_PHASE:
      //TODO:
      break;

  }

}
