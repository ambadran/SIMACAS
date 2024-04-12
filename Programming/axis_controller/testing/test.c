
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
        printf("ERROR: Unsupported direction value <%d>, expected 0 or 1\n", value.long_int);
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


