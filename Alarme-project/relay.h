#include "pins.h"

// Aciona relé
void triggerRelay( bool value ) {
  
  digitalWrite(RELAY, value);
  
}
