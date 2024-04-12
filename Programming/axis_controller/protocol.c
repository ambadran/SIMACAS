#include "project-defs.h"

static char line[LINE_BUFFER_SIZE];

static void protocol_execute_line(char* line) {

  if(line[0] == 0) {

    printf("\rNo Command, Only Enter Received..\n\n");
    
  } else {

    LINE_STATUS line_state = terminal_execute_line(line);

    if (line_state == LINE_PASSED) { printf("\rCommand passed..\n\n"); } 
    else if (line_state == LINE_FAILED) { printf("\rCommand Failed..\n\n"); }

  }

}

void protocol_main_loop(void) {

  uint8_t char_count = 0;
  uint8_t c;
  while(1) {

/* #ifdef DEBUG_MODE */
/*     while (c = uartGetCharacter(CONSOLE_UART, NON_BLOCKING)) { */
/* #else */
    while (c = uartGetCharacter(CONSOLE_UART, NON_BLOCKING)) {
/* #endif */


      // line end
      if ((c == '\n') || (c == '\r')) {
        line[char_count] = 0; // string termination character
        char_count = 0;

        // executing line!
        protocol_execute_line(line);

      // throw away whitespaces and control characters
      } else if (c <= ' '){
        // do nothing

      // buffer overflow
      } else if (char_count >= (LINE_BUFFER_SIZE-1)) {

        printf("BUFFER OVERFLOW!!!!");
        char_count = 0;

      // store uart_rx_buffer in line character array
      } else {

        line[char_count++] = c;

      }
    }

    // Other persistant tasks to be put here..

   
  }

}
