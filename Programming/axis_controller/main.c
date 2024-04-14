#include "project-defs.h"

INTERRUPT(STEPPER_TIMER_ISR, STEPPER_TIMER_INTERRUPT);
INTERRUPT(ULTRASONIC_TRIGGER_ISR, ULTRASONIC_TRIGGER_INTERRUPT);

void main(void) {

  // inits
  INIT_EXTENDED_SFR();
  EA = 1; // enable interrupts
  serialConsoleInitialise(
      CONSOLE_UART, 
      CONSOLE_SPEED, 
      CONSOLE_PIN_CONFIG
      );
  // initialize stepper_motor
  stepper_motor_init();

  // Awaiting User picocom
  // TODO: add a debug switch that switches on this code for debug mode or ignore this code for normal mode
  uint8_t initial_char = '0';
  while (initial_char != 's') {
    initial_char = uartGetCharacter(CONSOLE_UART, NON_BLOCKING);
  }
  uartSendBlock(CONSOLE_UART, "\rStarting..\n\n", 12, NON_BLOCKING);

  //initializing nrf24l01+ as a receiver device with one simpl function call
  nrf24_device(RECEIVER, RESET);

  // Main Routine
  protocol_main_loop();

}


