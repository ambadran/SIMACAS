#ifndef TERMINAL_H
#define TERMINAL_H

#define MAX_INT_DIGITS 8

#define MODE_FUNC_NONE 0
#define MODE_FUNC_MOVE 1

#define DIRECTION_BACKWARD 0
#define DIRECTION_FORWARD 1

/* typedef enum { */
/*   LINE_PASSED, */ 
/*   LINE_FAILED */
/* } LINE_STATUS */

/* typedef struct { */
/*   uint8_t direction;  // 0 for backward, 1 for forward */
/*   float distance;  // in meters */
/* } movement_command_t; */

uint8_t read_int(char *line, uint8_t *char_count, value_t *value_ptr);
uint8_t read_float(char *line, uint8_t *char_count, value_t *value_ptr); 

uint8_t terminal_execute_line(char* line);

#endif
