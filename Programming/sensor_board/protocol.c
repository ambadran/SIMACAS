#include "project-defs.h"

static char line[LINE_BUFFER_SIZE];
static LINE_STATUS line_state = LINE_EMPTY;

void protocol_main_loop(void) {

  uint8_t char_count = 0;
  uint8_t c;

  while (1) {
  
    /*** Get User Input ***/
    while (c = uartGetCharacter(CONSOLE_UART, NON_BLOCKING)) {

      // ECHO into terminal for user to see what is being typed, enter will be printed once command is executed
      uartSendCharacter(CONSOLE_UART, c, BLOCKING);

      // line end
      if ((c == '\n') || (c == '\r')) {
        line[char_count] = 0; // string termination character
        char_count = 0;

        /* // executing line! */
        /* protocol_execute_line(line); */
        uartSendCharacter(CONSOLE_UART, '\n', BLOCKING);
        line_state = LINE_READ;

      } else if (c <= ' '){
          // throw away whitespaces and control characters

      } else if (char_count >= (LINE_BUFFER_SIZE-1)) {

        // buffer overflow
        printf("BUFFER OVERFLOW!!!!");
        char_count = 0;

      } else {

        // store uart_rx_buffer in line character array
        line[char_count++] = c;

      }

    }
    /*** Got User Input!***/

    /*** Execute user Input ***/
    if (line_state == LINE_READ)
    if(line[0] == 0) {
      // do nothing?!
      printf("Device Responsive!\n\n");
      line_state = LINE_PASSED;
      
    } else {

      line_state = terminal_execute_line(line);
      if (line_state == LINE_PASSED) { printf("Command Passed..\n\n"); } 
      else if (line_state == LINE_FAILED) { printf("Command Failed..\n\n"); }

    }
    /*** User Input Executed! ***/


    /*** Reading All Sensors ***/
    
    /*** Read All Sensors! ***/


  }

}

