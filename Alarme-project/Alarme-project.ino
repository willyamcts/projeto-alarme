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
 *  - Ethernet
 *    
 */

# include "buzzer.h"
# include "pins.h"
# include "pir-sensor.h"
//# include "rfid.h"
# include "magnetic-sensor.h"
# include "ethernet.h"



// --------------------- Definitions ---------------------
//bool activePIR();
//bool checkRFID();

// Armazena estado do alarme (on/off);
bool alarmOn = false;

// Armazena o estado dos sensores;
bool action = false;


// --------------------- Setup ---------------------
void setup() {
  /*
   * Conf. pinos
   */
  pinMode(BUZZER, OUTPUT);
  pinMode(PIR_1, INPUT);
  pinMode(PIR_2, INPUT);
  pinMode(SWITCH_MAGNETIC_1, INPUT_PULLUP);
  pinMode(LED_BOARD, OUTPUT);

  /*
   * Define estados
   */
  digitalWrite(LED_BOARD, LOW);



  Serial.begin(9600);

  // Start RFID RC-522
  //SPI.begin();      // Inicia  SPI bus
//  mfrc522.PCD_Init();   // Inicia MFRC522


  // Start Ethernet W5500
  Ethernet.begin(mac, ip);
  server.begin(); //

// TODO: Iniciando alterando o valor de alarmOn devido ao retorno da função initServer
//  postForm("InIcIaNdO_-_", String(alarmOn) );
  
}






// deixar somente chamada de métodos de acionamento;
void loop() {


  Serial.println(alarmOn);

  // Start server web
  Serial.println("SERVER: ");
  Serial.println(initServer(alarmOn));
  
 // TODO: SERIAL PRINT Only
  if ( alarmOn == false) {
    Serial.print("Alarme desativado!! alarmOn = ");
    Serial.println(alarmOn);
  }

  // Veriifca os retornos dos módulos se o alarme estiver ativo;
  if ( alarmOn == true ) {
    checkStatusModules();
  }
  
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
    
    //if ( checkMagnetic() || activePIR() ) {
    if ( activePIR() ) {
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

        
// TODO: Serial print
Serial.print("DISPAROU action=true - PIR= ");
//Serial.print(digitalRead(PIR_1));
Serial.print(activePIR());
Serial.print(" - MAGNETICO= ");
//Serial.print(digitalRead(SWITCH_MAGNETIC_1));
Serial.println(checkMagnetic());

        // post data in google sheet
      //  postForm("PIR_Sensor", "");

        
    }
      
}



