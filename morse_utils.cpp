#include <Arduino.h>
#include "morse_utils.h"

void write_morse(int ch) {
  switch (ch) {
    case '.':
      write_state(unit, HIGH);
      write_state(unit, LOW);
      break;
  
    case '-':
      write_state(3.0*unit, HIGH);
      write_state(unit, LOW);
      break;
    
    case ' ':
      if (previous_character == '.' || previous_character == '-') {
        write_state(2.0*f_unit + (f_unit - unit), LOW);
        break;
      }
      write_state(3.0*f_unit, LOW);
      break;

    case '/':
    case '\n':
      if (previous_character == '.' || previous_character == '-') {
        write_state(6.0*f_unit + (f_unit - unit), LOW);
        break;
      }
      write_state(7.0*f_unit, LOW);
  }
  previous_character = ch;
}
