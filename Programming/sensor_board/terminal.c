#include "project-defs.h"

// reads int only not floats
bool read_int(char *line, uint8_t* char_count, int16_t* int_value_ptr) {

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
    *int_value_ptr = -lival;
  } else {
    *int_value_ptr = lival;
  }

  *char_count = ptr - line - 1; // Set char_counter to next statement
 
  return (true);
}


LINE_STATUS terminal_execute_line(char* line) {

  uint8_t char_count = 0;
  char letter;
  // temporary int value that gets read from the terminal then assigned to another variable of any other c component
  uint16_t int_value;


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

      case 'I':
        // testing reading int argument
        if (!read_int(line, &char_count, &int_value)) {
          printf("Bad integer Number Format\n");
          return LINE_FAILED;
        }

        printf("Read INT value: %d\n", int_value);
        break;

      case 'L':
        // Printing OUT the TEMT6000 sensor
        printf("TEMT6000:%d\n", get_temt6000_last_value());

        break;
 
      case 'T':
        // Printing OUT the DHT11 sensor
        printf("DHT11 Temperature:%d\n", get_dht11_temp_last_value());

        break;

      case 'H':
        // Printing OUT the DHT11 sensor
        printf("DHT11 Humidity:%d\n", get_dht11_humidity_last_value());

        break;


      case 'M':
        // Printing OUT the MQ135 sensor
        printf("MQ135:%d\n", get_mq135_last_value());

        break;
   
      case 'S':
        // Printing OUT the Soil Moisture sensor
        printf("SoilMoisture:%d\n", get_soil_moisture_last_value());

        break;

      default:
        printf("Unknown Command!\n");
        return LINE_FAILED;
    }



  }
  /* Parsing complete! (and non mode funcs are executed too)*/ 


  /* [ Step 3: Error checking ] */
  /* Error Checking Complete! */


  /* [ Step 4: Execute! ] */
  /* Modal funcs got executed! :D */

  return LINE_PASSED;


}

