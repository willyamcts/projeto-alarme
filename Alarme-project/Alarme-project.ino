/*
 * Author: 
 *  Willyam Castro
 * 
 * Description: 
 *  This project is one basic residence alarm;
 * 
 * 
 * Components: 
 *  - Nodecu ESP8266 12
 *  - LED RGB
 *  - Presence Infra Red module
 *	- Magnético
 *  - Relé 5v
 */

# include "pins.h"
# include "wifi.h"
# include "firebase.h"
# include "pir-sensor.h"
# include "magnetic-sensor.h"
# include "relay.h"
# include "buzzer.h"
# include "led.h"
//# include "rfid.h"



// --------------------- Definitions ---------------------


// Set data Firebase;
#define FIREBASE_HOST "...firebaseio.com"
#define FIREBASE_AUTH "16vw..."


// Set data connection wireless;
#define WIFI_SSID "VIVO-29A9"
#define WIFI_PASSWORD "C9D3BD29A9"


/*
***REMOVED***
***REMOVED***
*/

/*
#define WIFI_SSID "VIVOFIBRA-D6D8"
#define WIFI_PASSWORD "R041215W"
*/

// Armazena estado do alarme (on/off);
bool alarmOn;

// Armazena o estado dos sensores;
bool action = false;

bool alarmOnTemp;

// Armazena hora do disparo (millis);
unsigned long currentMillis = 0; // 0

// Intervalor de tempo para consultar o estado do alarme
const int timeCheck = 1000 * 15; // = 15 segundos

// "tempo de acionamento" do relé, em ms;
//	Conf: Altera somente o último número, mantém o [60000]
//      Em minutos
const long triggerTime = 15000; //60000 * 2;





// --------------------- Setup ---------------------
void setup() {

  Serial.begin(9600);
  
  /*
   * Conf. pinos
   */
  pinMode(LED_BOARD, OUTPUT);
  pinMode(PIR_1, INPUT);
  pinMode(SWITCH_MAGNETIC_1, INPUT_PULLUP); //INPUT_PULLUP);
  pinMode(RELAY, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_BLUE, OUTPUT);

/*
  pinMode(blue, OUTPUT);
  pinMode(green, OUTPUT);
  pinMode(red, OUTPUT);
*/

  /*
   * Define states
   */
  digitalWrite(LED_BOARD, LOW);
  digitalWrite(RELAY, LOW);




/*
  // Start RFID RC-522
  SPI.begin();      // Inicia  SPI bus
  mfrc522.PCD_Init();   // Inicia MFRC522
*/

/*
  // Start Ethernet W5500
  Ethernet.begin(mac, ip);
  server.begin(); //

  initServer(0);
*/
  ledConnectingWifi(true);
  connectWifi(WIFI_SSID, WIFI_PASSWORD);
  // Reconnect AP case is it disconnected;
  WiFi.setAutoReconnect(true);
  ledConnectingWifi(false);

  
  initFirebase(FIREBASE_HOST, FIREBASE_AUTH);

  alarmOn = checkDBAlarm("", "");
  alarmOnTemp = alarmOn;

  Serial.print("AlarmOn inicial: "); Serial.println(alarmOn);
  
}





unsigned long checkDBMillis = millis();

bool temp;


void loop() {
  //digitalWrite(LED_BOARD, HIGH);

  if ( millis() > (checkDBMillis + timeCheck) ) {
    checkDBMillis = millis();

    alarmOnTemp = checkDBAlarm("", "");

    //if ( alarmOnTemp == 0 || alarmOnTemp == 1 ) {
      
      if ( alarmOnTemp != alarmOn ) {
        alarmOn = alarmOnTemp;
  
        // desable relay case alarm change to off
        if (digitalRead(RELAY)) {
          triggerRelay(false);
        }
  
      }
      
    //} else {
//      low();
    //}

    //TODO: funcao correta comentada;
    //ledConsultingDB(alarmOn);
    ledIndicateStateAlarm(alarmOn);
  }
  
  //delay(250);
  //digitalWrite(LED_BOARD, HIGH);

/*
  if ( alarmOn != checkDBAlarm("", "") ) {
    
    // trigger buzzer/LED RGB;
    //enableDesableAlarm(alarmOn);

    alarmOn = !alarmOn;


    // TODO: Value alarmOn
    Serial.print("ALARME ON = "); Serial.println(alarmOn);

  
    enableDesableAlarm(alarmOn);
  }
*/

/*
  // trigger buzzer/LED RGB;
  if ( alarmOn != alarmOnTemp ) {
    enableDesableAlarm(alarmOn);
  }
*/

  // Veriifca os retornos dos módulos se o alarme estiver ativo (= 1);
  if ( alarmOn ) {
    checkStatusModules();
  }


  digitalWrite(LED_BOARD, !HIGH);
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

    // Serial.println(checkMagnetic());

    if ( checkMagnetic() ) {
        action = true;
        sensor = "Magnetico";
        local = "Porta traseira";      
  
    } else if ( activePIR() ) {
      action = true;
      sensor = "Presença Infravermelho";
      local = "Ferramentas";

    }
    
    else {
      
      action = false;
    } 


    Serial.print(".");

  //Desabilita relé após o tempo definido em triggerTime;
	if ( currentMillis != 0 && (millis() - currentMillis >= triggerTime) ) {

    // TODO: Remover
    Serial.print("MILLIS: ");
    Serial.println(currentMillis);

    // Atualiza o tempo atual caso algum sensor estiver acionado;
    if ( action == true ) {
      currentMillis = millis(); 
      
    } else {
      Serial.println("Desabilitando relé..."); 

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

  ledShooting();
  
  triggerRelay(1);


// TODO: Serial print
Serial.print("DISPAROU action=true");
Serial.print(" - Sensor: "); Serial.print(sensor);
Serial.print(" - Info: "); Serial.print(local);


  //post data to realtime database
  postData(local);
/*
  // post data in google sheet
  postForm(sensor, local);
*/
}

