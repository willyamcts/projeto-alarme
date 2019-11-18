
# include "pins.h"

// Check PIR sensor, @return boolean, true = active;
bool activePIR() {

  if ( digitalRead(PIR_1) == HIGH ) {
    return true;
  } else {
    return false;
  }

}
