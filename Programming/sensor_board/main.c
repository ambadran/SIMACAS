#include "project-defs.h"


/**** MUST DECLARE INTERRUPTS IN MAIN.C FILE AS PER SDCC REQUIREMENT ****/



void main(void) {

  // Inits / Deinits
  INIT_EXTENDED_SFR();
  serialConsoleInitialise(
      CONSOLE_UART, 
      CONSOLE_SPEED, 
      CONSOLE_PIN_CONFIG
      );
  adcInitialise(ADC_ALIGN_RIGHT, DISABLE_INTERRUPT);
  mq135_init();
  dht11_init();
  temt6000_init();
  soil_moisture_init();

  EA = 1; // enable interrupts

  // Main Routine
  protocol_main_loop();

}
