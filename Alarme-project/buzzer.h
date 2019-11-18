
# include "pins.h"

//int BUZZER = 7;

void danied() {
  tone(BUZZER, 150, 300);
  delay(300);
  tone(BUZZER, 100, 700);
  delay(500);
}

void accept() {
  delay(450);
  for (int i=0; i<2; i++) {
  tone(BUZZER, 475, 75);
  delay(90);
  tone(BUZZER, 425, 100);    
  }
  delay(500);
}

// Shoot buzzer alarm;
void shoot(){  
  tone(BUZZER, 500, 175);
  delay(250);
  tone(BUZZER, 500, 175);
}




void other() {  
  //tone(BUZZER, 150, 100);
  //delay(100);
  /*tone(BUZZER, 475, 200);
  delay(200);
  tone(BUZZER, 425, 350);
  */
  
  /*
  delay(750);
  tone(BUZZER, 500, 175);
  delay(250);
  tone(BUZZER, 500, 175);
  */
}

