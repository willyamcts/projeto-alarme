
# include "pins.h"

bool checkMagnetic() {

  if ( digitalRead(SWITCH_MAGNETIC_1) == HIGH) {    
    return true;
  } else {
    return false;
  }
  
}

