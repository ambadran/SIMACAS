#include "project-defs.h"

static volatile ULTRASONIC_STATUS ultrasonic_status = ULTRASONIC_INACTIVE;
static volatile uint16_t last_up_time = 0;

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
 * 2- switch on trigger pin and enable software flag trigger_pin_flag
 * 3- upon timer interrupt, if trigger_pin_flag, switch off trigger pin, reset trigger_pin_flag and start catch_signal routine
 *
 * as of this moment I am not sure how the capture mode works exactly, 
 * when it is set on PWM_CAPTURE_ON_RISING_EDGE, does it auto-magically give me the counterValue in the interrupt as the counted value from the previous rising to the now rising??
 * if so then the next steps would be
 *
 * 4- enable software flag awaiting_echo in another timer to wait 30ms, if no trigger is catched then global ultrasonic_status variable is set to ULTRASONIC_INACTIVE
 * 5- capture module interrupt sets the counted value and lowers the awaiting_echo flag and sets the global ultrasonic_status to ULTRASONIC_ACTIVE
 * */
void ultrasonic_init(void) {

  startTimer(
      ULTRASONIC_TRIGGER_TIMER,
      frequencyToSysclkDivisor(ULTRASONIC_TRIGGER_ON_TIME),
      DISABLE_OUTPUT, 
      ENABLE_INTERRUPT, 
      FREE_RUNNING
      );

}

ULTRASONIC_STATUS ultrasonic_get_distance(uint16_t* distance) {

  *distance = last_up_time*ULTRASONIC_COUNTER_TO_CM;
  return ultrasonic_status;

}

INTERRUPT(ULTRASONIC_TRIGGER_ISR, ULTRASONIC_TRIGGER_INTERRUPT) {


}

