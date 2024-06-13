#include "project-defs.h"


/**** MUST DECLARE INTERRUPTS IN MAIN.C FILE AS PER SDCC REQUIREMENT ****/


static GpioConfig test_pin = GPIO_PIN_CONFIG(5, 4, GPIO_BIDIRECTIONAL_MODE);

void main(void) {

  // Inits / Deinits
  INIT_EXTENDED_SFR();
  EA = 1; // enable interrupts
  serialConsoleInitialise(
      CONSOLE_UART, 
      CONSOLE_SPEED, 
      CONSOLE_PIN_CONFIG
      );

  gpioConfigure(&test_pin);

  // Main Routine
  while (1) {

    printf("LED: %f \r", gpioRead(&test_pin));


  }




}
