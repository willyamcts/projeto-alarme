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
 *  - Ethernet
 *  - Presence Infra Red module
 *	- Magnético
 *  - Relé 5v
 *  - Buzzer
 */

# include "pins.h"
# include "ethernet.h"
# include "pir-sensor.h"
# include "magnetic-sensor.h"
# include "relay.h"
# include "buzzer.h"
//# include "rfid.h"



// --------------------- Definitions ---------------------

// Armazena estado do alarme (on/off);
bool alarmOn = false;

// Armazena o estado dos sensores;
bool action = false;

bool alarmOnTemp;

// Armazena hora do disparo (millis);
unsigned long currentMillis = 0; // 0

// "tempo de acionamento" do relé, em ms;
//	Conf:
//      Altera somente o último número, mantém o [60000]
const long triggerTime = 15000; //60000 * 2;


// --------------------- Setup ---------------------
void setup() {
  
  /*
   * Conf. pinos
   */
  pinMode(LED_BOARD, OUTPUT);
  pinMode(PIR_1, INPUT);
  pinMode(SWITCH_MAGNETIC_1, INPUT_PULLUP);
  pinMode(RELAY, OUTPUT);
  pinMode(BUZZER, OUTPUT);

  /*
   * Define estados
   */
  digitalWrite(LED_BOARD, LOW);
  digitalWrite(RELAY, LOW);


  Serial.begin(9600);

  // Start RFID RC-522
  // SPI.begin();      // Inicia  SPI bus
  // mfrc522.PCD_Init();   // Inicia MFRC522


  // Start Ethernet W5500
  Ethernet.begin(mac, ip);
  server.begin(); //

  initServer(0);
}





void loop() {
  alarmOnTemp = alarmOn;

  // enable/disable by web;
  alarmOn = initServer(alarmOn);

  // trigger buzzer;
  if ( alarmOn != alarmOnTemp ) {
    enableDesableAlarm(alarmOn);
  }

  // TODO: Value alarOn
  Serial.print("ALARME ON = "); Serial.println(alarmOn);

 // TODO: SERIAL PRINT Only
  if ( alarmOn == false) {
    
    // Desabilita relé
    triggerRelay(false);
    Serial.print("Alarme desativado!! alarmOn = "); Serial.println(alarmOn);
  }

  // Veriifca os retornos dos módulos se o alarme estiver ativo (= 1);
  if ( alarmOn == true ) {
    checkStatusModules();
  }

}





void enableDesableAlarm(bool state) {
  
  if ( state ) {
    // liga LED da placa;
    digitalWrite(LED_BOARD, HIGH);
    // disparo sonoro de indicação no buzzer;
    accepted();
    delay(1);

  } else {
    // desliga o LED da placa:
    digitalWrite(LED_BOARD, LOW);
    // disparo sonoro de indicação no buzzer;
    danied();
  }
  
}




    // Pegar módulo a módulo em um array, varrer e disparar caso alguma posicao
    //  seja true;

void checkStatusModules() {

    String sensor = "";
    String local = "";

    if ( checkMagnetic() ) {
        action = true;
        sensor = "Magnetico";
        local = "Porta_traseira";        
        
    } else if ( activePIR() ) {
      action = true;
      sensor = "PIR";
      local = "Ferramentas";
       
    } else {
      
      action = false;
    }


Serial.print("MILLIS: ");
Serial.println(currentMillis);

  //Desabilita relé após o tempo definido em triggerTime;
	if ( currentMillis != 0 && (millis() - currentMillis >= triggerTime) ) {

    // Atualiza o tempo atual caso algum sensor estiver acionado;
    if ( action == true ) {
      currentMillis = millis(); 
      
    } else {      
Serial.println("Funcao relé");

      // Desabilita relé
		  triggerRelay(false);
		
		  // Retorna ao valor incial para uso posterior, atendendo a linha 186;
		  currentMillis = 0;
    
      delay(1);
    }
	}
	
	


    // Shoot from action = true;
    if ( action == true ) {	

  		if ( currentMillis == 0 ) {
  			currentMillis = millis();
  		}

      // Shoot functions trigger;
      shoot(sensor, local);        
    }	
}



void shoot(String sensor, String local) {

  // liga LED da placa
  digitalWrite(LED_BOARD, LOW);
  digitalWrite(LED_BOARD, HIGH);
		
  // Dispara dispositivo sonoro de teste (buzzer);        
  //shootBuzzer();

  triggerRelay(1);


// TODO: Serial print
Serial.print("DISPAROU action=true");
Serial.print(" - Sensor: "); Serial.print(sensor);
Serial.print(" - Info: "); Serial.print(local);

  // post data in google sheet
  postForm(sensor, local);

	}

