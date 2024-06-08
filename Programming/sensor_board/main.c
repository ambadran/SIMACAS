#include "project-defs.h"


/**** MUST DECLARE INTERRUPTS IN MAIN.C FILE AS PER SDCC REQUIREMENT ****/



void main(void) {

  // Inits / Deinits
  INIT_EXTENDED_SFR();
  EA = 1; // enable interrupts
  serialConsoleInitialise(
      CONSOLE_UART, 
      CONSOLE_SPEED, 
      CONSOLE_PIN_CONFIG
      );

  // Main Routine
  

}
