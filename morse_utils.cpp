#include <Arduino.h>
#include "morse_utils.h"

void write_morse(int ch) {
  switch (ch) {
    case '.':
      write_state(unit, HIGH);
      write_state(unit, LOW);
      break;
  
    case '-':
      write_state(3*unit, HIGH);
      write_state(unit, LOW);
      break;

    case '/':
      write_state(6*unit, LOW);
    
    case ' ':
      write_state(2*unit, LOW);
  }
}
