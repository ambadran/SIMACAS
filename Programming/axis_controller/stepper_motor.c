#include "project-defs.h"

static GpioConfig stepper_enable_pin = GPIO_PIN_CONFIG(STEPPER_MOTOR_PORT, STEPPER_ENABLE_PIN, GPIO_BIDIRECTIONAL_MODE);
static GpioConfig stepper_dir2_pin = GPIO_PIN_CONFIG(STEPPER_MOTOR_PORT, STEPPER_DIR2_PIN, GPIO_BIDIRECTIONAL_MODE);
static volatile GpioConfig stepper_step_pin = GPIO_PIN_CONFIG(STEPPER_MOTOR_PORT, STEPPER_STEP_PIN, GPIO_BIDIRECTIONAL_MODE);
static GpioConfig stepper_dir1_pin = GPIO_PIN_CONFIG(STEPPER_MOTOR_PORT, STEPPER_DIR1_PIN, GPIO_BIDIRECTIONAL_MODE);

static volatile uint16_t step_counter = 0;
static volatile uint8_t stepper_active = 0;

void stepper_motor_init(void) {

  // GPIOs init
  gpioConfigure(&stepper_enable_pin);
  gpioWrite(&stepper_enable_pin, STEPPER_DISABLE);
  gpioConfigure(&stepper_dir2_pin);
  gpioConfigure(&stepper_step_pin);
  gpioConfigure(&stepper_dir1_pin);

  // Timer init
	startTimer(
		STEPPER_TIMER, 
		frequencyToSysclkDivisor(DEFAULT_STEPPER_FREQUENCY),
		DISABLE_OUTPUT, 
		ENABLE_INTERRUPT, 
		FREE_RUNNING
	);

}

void stepper_motor_set_freq(uint16_t frequency) {
  /* stopTimer(STEPPER_TIMER); */
	startTimer(
		STEPPER_TIMER, 
		frequencyToSysclkDivisor(frequency),
		DISABLE_OUTPUT, 
		ENABLE_INTERRUPT, 
		FREE_RUNNING
	);
}

void stepper_motor_move(movement_type_t movement_type, int16_t distance) {

  if (movement_type == MOVE_ABSOLUTE) {

    printf("TODO: not yet supported!\n");


  } else if (movement_type == MOVE_RELATIVE) {

    gpioWrite(&stepper_enable_pin, STEPPER_ENABLE);

    if (distance < 0) {
      distance = -distance;
      gpioWrite(&stepper_dir1_pin, STEPPER_CLOCKWISE_DIR);
      gpioWrite(&stepper_dir2_pin, STEPPER_ANTICLOCKWISE_DIR);
    } else if (distance > 0) {
      gpioWrite(&stepper_dir1_pin, STEPPER_ANTICLOCKWISE_DIR);
      gpioWrite(&stepper_dir2_pin, STEPPER_CLOCKWISE_DIR);
    }

    step_counter = distance * STEPPER_CM_TO_STEPS;
    stepper_active = 1;
    printf("Steps to move: %d \n", step_counter);

  }

}

uint8_t get_stepper_state(void) {

  return stepper_active;

}


INTERRUPT(STEPPER_TIMER_ISR, STEPPER_TIMER_INTERRUPT) {

  if (step_counter) {

    step_counter--;
    gpioToggle(&stepper_step_pin);

  } else if (stepper_active) {

    gpioWrite(&stepper_enable_pin, STEPPER_DISABLE);
    stepper_active = 0;

  }

}

