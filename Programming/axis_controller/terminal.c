#include "project-defs.h"

uint8_t direction;  // 0 for backward, 1 for forward
float distance;  // in meters

// reads int only not floats
uint8_t read_int(char *line, uint8_t *char_count, value_t *value_ptr) {

  char *ptr = line + *char_count; // the variable that will extract the next digit from
  unsigned char c; // the variable that will hold each digit

  c = *ptr++;  // extracting the next digit!

  // catching negative values :)
  bool isnegative = false;
  if (c == '-') {
    isnegative = true;
    c = *ptr++;

  // incrementing pointer anyway if +ve sign is there
  } else if (c == '+') {
    c = *ptr++;
  }

  // Algorithm to convert char digits to int
  uint32_t intval = 0;
  uint8_t ndigit = 0;  // current digit read index
  int8_t exp = 0;  // NOT uint8_t as this must hold +ve and -ve numbers
  while (1) {

    c -= '0';
    if (c <= 9) {
      ndigit++;
      if (ndigit <= MAX_INT_DIGITS) {

        intval = (((intval << 2) + intval) << 1) + c; // intval*10 + c
                                                      //
      } else {

        exp++;  // Drop overflow digits
      }

    } else {
      // encountered a non integer character
      break;
    }
    c = *ptr++;
  }

  // Return if no digits have been read.
  if (!ndigit) { return(false); };

  long int lival;
  lival = (long int)intval;
  
  if (exp > 0) {
    do {
      lival *= 10.0;
    } while (--exp > 0);
  } 

  // Assign floating point value with correct sign.    
  if (isnegative) {
    value_ptr->long_int = -lival;
  } else {
    value_ptr->long_int = lival;
  }

  *char_count = ptr - line - 1; // Set char_counter to next statement
 
  return (true);
}

// arguments are the line of terminal code itself
// char_count_ptr to increment it as we read and be able to use it later in the terminal_execute_line after this func is done
// float_ptr to assign the output of this function to it
// THIS FUNCTION RETURN WHETHER IT SUCCESSFULYY READ THE FLOAT OR NOT, AKA (0 OR 1) 
uint8_t read_float(char *line, uint8_t *char_count, value_t *value_ptr) {

  char *ptr = line + *char_count; // the variable that will extract the next digit from
  unsigned char c; // the variable that will hold each digit

  c = *ptr++;  // extracting the next digit!

  // catching negative values :)
  bool isnegative = false;
  if (c == '-') {
    isnegative = true;
    c = *ptr++;

  // incrementing pointer anyway if +ve sign is there
  } else if (c == '+') {
    c = *ptr++;
  }

  // extracting ALL digits from line until a non-digit character is encountered to variable intval
  // variable exp will hold value x (in *10^x) which will be applied to intval after it's extracted
  // if x is negative, then the value is a float and that decimals will appear in the number 
  // if x is positive, then the value is has digits more than MAX_INT_DIGITS and this will mean
  // that after multiplication all digits after MAX_INT_DIGITS are just zeros
  // exp will be decremented with every digit registerd if '.' is encountered
  // exp will be incremented if ndigit > MAX_INT_DIGITS and still no '.' encountered
  uint32_t intval = 0;
  int8_t exp = 0;  // NOT uint8_t as this must hold +ve and -ve numbers
  uint8_t ndigit = 0;  // current digit read index
  bool isdecimal =  false;  // to be set if '.' is encountered
  while (1) {

    c -= '0';  // converting ascii number of digit to the digit itself >:)
    if (c <= 9) {
      ndigit++;
      if (ndigit <= MAX_INT_DIGITS) {
        if (isdecimal) {
          exp--; 
        }
        intval = (((intval << 2) + intval) << 1) + c; // intval*10 + c
                                                      //
      } else {
        if (!(isdecimal)) { 
          exp++;  // Drop overflow digits
        }
      }
    } else if (c == (('.'-'0') & 0xff)  &&  !(isdecimal)) {
      isdecimal = true;

    } else {
      break;

    }
    c = *ptr++;
  }
   
  // Return if no digits have been read.
  if (!ndigit) { return(false); };
  
  // Convert integer into floating point.
  float fval;
  fval = (float)intval;
  
  // Apply decimal. Should perform no more than two floating point multiplications for the
  // expected range of E0 to E-4.
  if (fval != 0) {
    while (exp <= -2) {
      fval *= 0.01; 
      exp += 2;
    }
    if (exp < 0) { 
      fval *= 0.1; 
    } else if (exp > 0) {
      do {
        fval *= 10.0;
      } while (--exp > 0);
    } 
  }

  // Assign floating point value with correct sign.    
  if (isnegative) {
    value_ptr->float_ = -fval;
  } else {
    value_ptr->float_ = fval;
  }

  *char_count = ptr - line - 1; // Set char_counter to next statement
 
  return (true);
}



uint8_t terminal_execute_line(char* line) {

  // Mode functions are functions that don't pass argument
  // with command word
  // instead they pass function mode with command letter
  // and then pass argument in argument letter 
  // e.g- 'G' -> cpp funcs
  uint8_t mode_func_ind = MODE_FUNC_NONE;

  uint8_t char_count = 0;
  char letter;
  value_t value;  // union of either float or long int depending on user input
  uint8_t int_value; // meant to be assigned to x in Gx.0
  uint16_t mantissa; // not really the mantissa defined by xc8 compiler, it's what the gerber code named it. It's basically the mantissa without the whole number digits.
  while (line[char_count] != 0) {

    // Reading letter argument, aka which function to execute
    letter = line[char_count];

    // incrementing char_count for next iteration
    char_count++;

    // IMP: capital letters are command letters
    //      small letters are argument letters
    // Checking letter is a letter
    // checking if it's before A or after z or any character in between Z and a in the ascii table
    if ( letter < 'A' || letter > 'z' || (letter > 'Z' && letter < 'a')) {
      printf("Expected a letter!\n"); 
      return LINE_FAILED;
    }


    /* Identifying function and printing the resultant execution */
    switch(letter) {

      case 'A':
        // testing reading int argument
        if (!read_int(line, &char_count, &value)) {
          printf("Bad integer Number Format\n");
          return LINE_FAILED;
        }

        printf("Read INT value: %d\n", (int)value.long_int);
        break;

      case 'B':

        // testing reading float argument
        if (!read_float(line, &char_count, &value)) {
          printf("Bad float Number Format\n");
          return LINE_FAILED;
        }

        printf("Read Float value: %3f\n", value.float_);
        break;

      case 'M':
        // Move Command:
        // first argument is the direction 0 for backwards, 1 for forward. e.g M0 or M1
        // second argument is the value to move in meters (f for float). e.g- M0 f0.3 -> move backwards 0.3meters
        // reading int argument
        if (!read_int(line, &char_count, &value)) {
          printf("Bad int Number Format\n");
          return LINE_FAILED;
        }

        // for catching second argument, activating error checking and execute routines
        mode_func_ind = MODE_FUNC_MOVE;

        // capturing direction value, will check it, execute it later
        direction = (uint8_t)value;

        break;

      default:

        switch(letter) {

          case 'i':
            // reading int argument for a multi-argument command
            if (!read_int(line, &char_count, &value)) {
              printf("Bad int Number Format\n");
              return LINE_FAILED;
            }

            switch(mode_func_ind) {
              case MODE_FUNC_MOVE:
                distance = (uint8_t)value;
            }

            break;

          case 'f':
            // reading float argument for a multi-argument command
            if (!read_float(line, &char_count, &value)) {
              printf("Bad float Number Format\n");
              return LINE_FAILED;
            }

            switch(mode_func_ind) {
              case MODE_FUNC_MOVE:
                distance = (float)value;
            }

            break;

        }
        printf("Command Letter Not Implemented\n");
        return LINE_FAILED;

    }

  }
  /* Parsing complete! (and non mode funcs are executed too)*/

  // The rest of the steps are only Applicable for Mode funcs
  if (mode_func_ind == MODE_FUNC_NONE) {
    return LINE_PASSED;
  }

  /* [ Step 3: Error checking ] */
  switch(mode_func_ind) {

    case MODE_FUNC_MOVE:
      // Direction Checks
      if (direction != DIRECTION_BACKWARD && direction != DIRECTION_FORWARD) {
        printf("ERROR: Unsupported direction value <%d>, expected 0 or 1\n", value);
        return LINE_FAILED;
      }

      // Distance value Checks
      if (distance < 0) {
        printf("ERROR: can't move a negative distance!\n");
        return LINE_FAILED;
      }

  }

  /* [ Step 4: Execute! ] */
  switch (mode_func_ind) {

    case MODE_FUNC_MOVE:
      stepper_motor_move((uint8_t)direction, (float)distance);

      printf("Executing Move Direction: %d, Dictance: %3f\n", (uint8_t)direction, (float)distance);

      break;

  }

  return LINE_PASSED;

}
