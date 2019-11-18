/*
 * Author: 
 *  Willyam Castro
 * 
 * Description: 
 *  This project is one basic residence alarm;
 * 
 * 
 * Components: 
 *  - Arduino Uno
 *  - Presence Infra Red module
 *  - Switch Magnetic module
 *  - Buzzer 
 *  - Relay 12v
 *    
 */

# include "buzzer.h"
# include "pins.h"
# include "pir-sensor.h"
# include "rfid.h"
# include "magnetic-sensor.h"

# define BUTTON 8
/*
# define BUZZER 2
 # define PIR_1 7
# define PIR_2 10
# define LED_BOARD 13
*/

// --------------------- Definitions ---------------------
//bool activePIR();
//bool checkRFID();

// Armazena estado do alarme (on/off);
bool alarmOn = false;
// Usado no botão de acionamento;
bool buttonState;
int x = 0;

// Armazena o estado dos sensores;
bool action = false;


// --------------------- Setup ---------------------
void setup() {
  /*
   * Conf. pinos
   */
  pinMode(BUTTON, INPUT); // or INPUT_PULLUP
  pinMode(BUZZER, OUTPUT);
  pinMode(PIR_1, INPUT);
  pinMode(PIR_2, INPUT);
  pinMode(SWITCH_MAGNETIC_1, INPUT_PULLUP);
  pinMode(LED_BOARD, OUTPUT);

  /*
   * Define estados
   */
  digitalWrite(LED_BOARD, LOW);
  digitalWrite(BUTTON, HIGH);



  Serial.begin(9600);

  // Start RFID RC-522
  SPI.begin();      // Inicia  SPI bus
  mfrc522.PCD_Init();   // Inicia MFRC522

  
  buttonState = digitalRead(BUTTON);
}






// deixar somente chamada de métodos de acionamento;
void loop() {
  //Serial.println(buttonState);

  // Falhando ao verificar se está ativo ou não;
  //if ( digitalRead(BUTTON) == LOW ) {

Serial.print("\n");

 // SERIAL PRINT
  if ( alarmOn == false) {
    Serial.print("Alarme desativado!! alarmOn = ");
    Serial.println(alarmOn);
  } else {
    //Serial.println();
    //Serial.print("Alarme ativado!! alarmOn = ");
    //Serial.println(alarmOn);
  }


  // Faz a leitura do módulo RFID/inserção de card;
  if ( checkRFID() ) { //&& alarmOn == false ) {
    alarmOn = !alarmOn;
    enableDesableAlarm(alarmOn);
    delay(3000);
  }

  // Se ativo
    // Verifica módulo X...

/*
  if ( alarmOn == true && digitalRead(SWITCH_MAGNETIC_1) == HIGH ) {
    shoot();
  }
*/

  // Veriifca os retornos dos módulos se o alarme estiver ativo;
  
  if ( alarmOn == true ) {
    checkStatusModules();
  }
  

  // Teste da Serial;
//  if ( alarmOn == true && x%2 != 0 ) {}
  
}




void enableDesableAlarm(bool state) {
  
  if ( state ) {
    // liga LED da placa;
    digitalWrite(LED_BOARD, HIGH);
    // disparo sonoro de indicação no buzzer;
    accept();

  } else {
    // desliga o LED da placa:
    digitalWrite(LED_BOARD, LOW);
    // disparo sonoro de indicação no buzzer;
    danied();
  }
  
}



void checkStatusModules() {
    
    if ( checkMagnetic() || activePIR() ) {
        action = true;        
        
    } else {
      action = false;
    }

    // Pegar módulo a módulo em um array, varrer e disparar caso alguma posicao
    //  seja true;



    // Shoot from action = true;
    if ( action == true ) {
        // liga LED da placa
        digitalWrite(LED_BOARD, LOW);
        digitalWrite(LED_BOARD, HIGH);
        // Dispara dispositivo sonoro;
        
        shoot();
        

Serial.print("DISPAROU - PIR= ");
//Serial.print(digitalRead(PIR_1));
Serial.print(activePIR());
Serial.print(" - MAGNETICO= ");
//Serial.print(digitalRead(SWITCH_MAGNETIC_1));
Serial.print(checkMagnetic());
        
    }
      
}



