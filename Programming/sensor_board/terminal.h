#ifndef TERMINAL_H
#define TERMINAL_H

#define MAX_INT_DIGITS 8

typedef enum {
  LINE_EMPTY,
  LINE_READ,
  /* LINE_PROCESS_IN_ORDER, */
  LINE_PASSED, 
  LINE_FAILED
} LINE_STATUS;

bool read_int(char *line, uint8_t *char_count, int16_t *value_ptr);
LINE_STATUS terminal_execute_line(char* line);

#endif
