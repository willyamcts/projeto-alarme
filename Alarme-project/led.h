
# include "pins.h"


/*
 * - Color default: White
 * 
 * - Procurando conexão wi-fi
 * - Consultou o db, valor do alarme ativo;
 * - Conectando Wi-fi*: Five blink Lilac;
 *    - Connected: Two little blink lilac. next color default;
 * - Alarme desabilitado*: Red;
 * - Alarme habilitado*: Green;
 * - Modo teste*: constant blink green;
 * - Disparado*: fast alternate in red, blue and green;
 */

void low() {  
  digitalWrite(LED_BLUE, LOW);
  digitalWrite(LED_GREEN, LOW);
  digitalWrite(LED_RED, LOW);
}

void standard() {
  digitalWrite(LED_BLUE, HIGH);
  digitalWrite(LED_GREEN, HIGH);
  digitalWrite(LED_RED, HIGH);
}

void pink() {
  digitalWrite(LED_BLUE, 75);
  digitalWrite(LED_GREEN, HIGH);
  digitalWrite(LED_RED, LOW);
}

void yellow() {
  digitalWrite(LED_BLUE, HIGH);
  digitalWrite(LED_GREEN, HIGH);
  digitalWrite(LED_RED, LOW);
}

// Lilás
void lilac() {
  digitalWrite(LED_BLUE, HIGH);
  digitalWrite(LED_GREEN, LOW);
  digitalWrite(LED_RED, HIGH);
}

void blue(){
  digitalWrite(LED_BLUE, HIGH);
  digitalWrite(LED_GREEN, LOW);
  digitalWrite(LED_RED, LOW);
}

void green() {
  digitalWrite(LED_BLUE, LOW);
  digitalWrite(LED_GREEN, HIGH);
  digitalWrite(LED_RED, LOW);
}

void red() {
  digitalWrite(LED_GREEN, LOW);
  digitalWrite(LED_BLUE, LOW);
  digitalWrite(LED_RED, HIGH);
}



void ledConnectingWifi(bool start) {
  if (start) {
    for (int i=0; i < 5; i++) {
      delay(250);
      low(); delay(250);
      lilac(); delay(200);
    }
  } else {
    standard(); delay(150);
    lilac(); delay(150);
    standard();
  }  
}



void ledDesable() {
  for (int i=0; i < 2; i++) {
    delay(250);
    low(); delay(250);
    red();
  }
}


void ledEnable() {
  for (int i=0; i < 2; i++) {
    delay(250);
    low(); delay(250);
    green();
  }
}


void ledShooting() {
  for( int i=0; i < 5; i++) {
    red(); delay(50);
    blue(); delay(50);
    green(); delay(50);
  }
}


void ledIndicateStateAlarm(int alarmState) {
  if(alarmState == 0) {
    ledDesable();
    
  } else if(alarmState == 1) {
    ledEnable();
    
  } else {
    ledDesable();
    ledEnable();
    standard();
  }
}



void ledConsultingDB(bool state) {
  low(); delay(300);
  pink(); delay(300);
  low(); delay(300);
  ledIndicateStateAlarm(state);
}

