
#include "project-defs.h"

static ULTRASONIC_STATUS ultrasonic_status = ULTRASONIC_INACTIVE;
volatile ULTRASONIC_PHASE ultrasonic_current_phase = 
static volatile uint32_t ultrasonic_timer_echo_ticks = 0;
static GpioConfig trigger_pin = GPIO_PIN_CONFIG(ULTRASONIC_TRIGGER_PORT, ULTRASONIC_TRIGGER_PIN, GPIO_BIDIRECTIONAL_MODE); 
static GpioConfig echo_pin = GPIO_PIN_CONFIG(ULTRASONIC_ECHO_PORT, ULTRASONIC_ECHO_PIN, GPIO_BIDIRECTIONAL_MODE);

/* AS PER REQUIRED FROM THE DOCUMENTATION IN `advpwm-hal.h` file */
#pragma save
// Suppress warning "unreferenced function argument"
#pragma disable_warning 85
void pwmOnCounterInterrupt(PWM_Counter counter, PWM_CounterInterrupt HAL_PWM_SEGMENT event) {
}

void pwmOnChannelInterrupt(PWM_Channel channel, uint16_t HAL_PWM_SEGMENT counterValue) {

}
#pragma restore

/* 
 * 1- start a timer of 10us
 * 2- enable software flag trigger_pin_flag
 * 3- if trigger pin 
 *
 * as of this moment I am not sure how the capture mode works exactly, 
 * when it is set on PWM_CAPTURE_ON_RISING_EDGE, does it auto-magically give me the counterValue in the interrupt as the counted value from the previous rising to the now rising??
 * if so then the next steps would be
 *
 * 4- enable software flag awaiting_echo in another timer to wait 30ms, if no trigger is catched then global ultrasonic_status variable is set to ULTRASONIC_INACTIVE
 * 5- capture module interrupt sets the counted value and lowers the awaiting_echo flag and sets the global ultrasonic_status to ULTRASONIC_ACTIVE
 * */
void ultrasonic_init(void) {

  // Setting trigger pin GpioConfig
  gpioConfigure(&trigger_pin);
  gpioWrite(&trigger_pin, 0);

  // Setting echo pin GpioConfig
  gpioConfigure(&echo_pin);

}

void static ultrasonic_send_trigger(void) {

  gpioWrite(&trigger_pin, 1);

  startTimer(
      ULTRASONIC_TIMER,
      frequencyToSysclkDivisor(ULTRASONIC_TRIGGER_ON_TIME),
      DISABLE_OUTPUT, 
      ENABLE_INTERRUPT, 
      FREE_RUNNING
      );

  ultrasonic_current_phase = ULTRASONIC_TRIGGER_SENT_PHASE;

}


void static ultrasonic_await_echo(void) {

  gpioWrite(&trigger_pin, 0);
  stopTimer(ULTRASONIC_TIMER);
  ultrasonic_current_phase = ULTRASONIC_AWAITING_ECHO_PHASE;

}

void processs_ultrasonic_phases(void) {
  switch (ultrasonic_current_phase) {
    case ULTRASONIC_TRIGGER_PHASE:

      ultrasonic_send_trigger();
      break;


    case ULTRASONIC_AWAITING_ECHO_PHASE:

      ultrasonic_await_echo();
      break;

  }
}

ULTRASONIC_STATUS ultrasonic_get_distance(uint16_t* distance) {

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

  if(ultrasonic_current_phase == ULTRASONIC_TRIGGER_SENT_PHASE) {

    ultrasonic_current_phase = ULTRASONIC_AWAITING_ECHO_RISE_PHASE;

  }

}

INTERRUPT(int_pin) {

  if(ultrasonic_current_phase == ULTRASONIC_AWAITING_ECHO_RISE_PHASE) {

    ultrasonic_current_phase

  }

}
