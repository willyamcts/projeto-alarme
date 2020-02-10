/*
 * Author: 
 *  Willyam Castro
 * 
 * Description: 
 *  This project is one basic residence alarm;
 * 
 * 
 * Components: 
 *  - NodeMCU ESP8266 12
 *  - LED RGB
 *  - Sensor de presença infravermelho
 *	- Sensor magnético p/ portas e janelas
 *  - Relé 5v
 */

# include "pins.h"
# include "wifi.h"
# include "firebase.h"
# include "pir-sensor.h"
# include "magnetic-sensor.h"
# include "relay.h"
//# include "buzzer.h"
# include "led.h"
//# include "rfid.h"
//# include "humidity_temperature-sensor.h"



// --------------------- Definitions ---------------------


// Set data Firebase;
#define FIREBASE_HOST "...firebaseio.com"
#define FIREBASE_AUTH "16v..."


***REMOVED***
***REMOVED***

/*
// Set data connection wireless;
#define WIFI_SSID "VIVO-29A9"
#define WIFI_PASSWORD "C9D3BD29A9"
*/

/*
***REMOVED***
***REMOVED***
*/

/*
#define WIFI_SSID "VIVOFIBRA-D6D8"
#define WIFI_PASSWORD "R041215W"
*/

/*
#define WIFI_SSID "Rede"
#define WIFI_PASSWORD "afgm6033"
*/




// Store alarm real status (on/off);
bool alarmOn;
// Store alarm status temporarily;
bool alarmOnTemp;

// Sensors status store;
bool action = false;

// Intervalor de tempo para consultar o estado do alarme
const int timeCheck = 1000 * 15; // = 15 segundos


// Armazena hora do disparo (millis);
unsigned long currentMillisTrigger = 0; // 0
// "tempo de acionamento" do relé, em ms;
//	Conf: Altera somente o último número (minutos),
//    mantém o [60000]
const long triggerTime = 15000; //60000 * 2;


// intervalo de tempo para atualizar o tempo que o alarme está habilitado;
//  Conf: Altera somente o último número, mantém o [60000]
unsigned long uptimeOn = 60000 * 1;
// Armazena o tempo em que está habilitado;
unsigned long currentMillisOn = 0;
// Armazena o tempo decorrido até ativação do alarme, para ser descontado de 
//    @currentMillisOn
unsigned long discountMillisOn = 0;





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
 *   // DHT
  pinMode(A0, INPUT_PULLUP);
delayMicroseconds(100);
*/

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

  ledConnectingWifi(true);
  connectWifi(WIFI_SSID, WIFI_PASSWORD);
  // Reconnect AP case is it disconnected;
  WiFi.setAutoReconnect(true);
  ledConnectingWifi(false);

  
  initFirebase(FIREBASE_HOST, FIREBASE_AUTH);
// TODO; NEW Firebase.Stream
//dbStream();

  delay(1000);
  // post hour and date from boot
  bootTime();

  // post initial value @uptime = 0
  postUptime();
  // post initial value @uptimeOn = 0
  postUptimeOn(0);

  alarmOn = checkDBAlarm();
  alarmOnTemp = alarmOn;

  Serial.print("AlarmOn inicial: "); Serial.println(alarmOn);

  // DHT
  //initDHT();
    
}

unsigned long checkDBMillis = millis();





void loop() {
  //digitalWrite(LED_BOARD, HIGH);

  // Check db in interval defined;
  if ( millis() > (checkDBMillis + timeCheck) ) {
    checkDBMillis = millis();

/*
// TODO: NEW
    if ( Firebase.available() ) {
      Serial.print("NOVA CONSULTA: "); Serial.println(checkDBAlarm("", ""));
    }
*/
    
    alarmOnTemp = checkDBAlarm();

    postUptime();

    // change var @alarmOn case changed in db
    if ( (alarmOnTemp == 0 || alarmOnTemp == 1) && alarmOnTemp != alarmOn ) {

// TODO: Entrando na função eventualmente mesmo com alarmOn == alarmOnTemp
Serial.print("alarmOnTemp = "); Serial.print(alarmOnTemp); Serial.print(", "); Serial.print(sizeof(alarmOnTemp)); Serial.print(" - "); 
Serial.print("alarmOn = "); Serial.print(alarmOn); Serial.print(", "); Serial.println(sizeof(alarmOn));

      alarmOn = alarmOnTemp;
  
      // desable relay case alarm change to off
      if (digitalRead(RELAY)) {
        triggerRelay(false);
        
      }

// TODO:
Serial.print("ALARMON = "); Serial.println(alarmOn);

      if (alarmOn) {
        discountMillisOn = millis();
      } else {
        // set time On 
        currentMillisOn = 0;
        postUptimeOn(currentMillisOn);
        
      }
      
    }

    //TODO: funcao correta comentada;
    //ledConsultingDB(alarmOn);
    ledIndicateStateAlarm(alarmOn);

    

  /// TODO: Teste DHT
  //readDHT();
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

      // @millis()
      //Atualiza tempo do alarme ativo @uptimeOn
    if ( millis() > ((currentMillisOn+discountMillisOn) + uptimeOn) ) {    
      currentMillisOn = millis() - discountMillisOn;
      postUptimeOn(currentMillisOn);
      
    }    
  }

  digitalWrite(LED_BOARD, !HIGH);
}





/*
    // Alterações ao mudar estado do alarme on/off
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
*/



    // Pegar módulo a módulo em um array, varrer e disparar caso alguma posicao
    //  seja true;
void checkStatusModules() {
  
  String sensor = "";
  String local = "";

  if ( checkMagnetic() ) {
    action = true;
    sensor = "Magnetico";
    local = "Porta traseira";      
  
  } else if ( activePIR() ) {
    action = true;
    sensor = "Presença Infravermelho";
    local = "Ferramentas";

  } else {      
    action = false;      
  }

  // TODO: ref para alarme ativo
  Serial.print(".");


  // @millis()
  //Desabilita relé após o tempo definido em @triggerTime;
	if ( currentMillisTrigger != 0 && (millis() - currentMillisTrigger >= triggerTime) ) {
    // TODO: Remover
    Serial.println(); Serial.print("MILLIS: "); Serial.println(currentMillisTrigger);

    // Atualiza o tempo atual caso algum sensor estiver acionado;
    if ( action == true ) {
      currentMillisTrigger = millis(); 
      
    } else {
      Serial.println("Desabilitando relé..."); 

      // Desabilita relé
		  triggerRelay(false);
		
		  // Retorna ao valor incial para uso posterior, atendendo a linha 186;
		  currentMillisTrigger = 0;
    
      delay(1);
    }
	}

    // Shoot from action = true;
  if ( action == true ) {
    currentMillisTrigger = millis();

      // Shoot functions trigger;
    shoot(sensor, local);
  }

}



  // Função acionada caso o alarme seja disparado;
void shoot(String sensor, String local) {

  // liga LED da placa
  digitalWrite(LED_BOARD, LOW);
  digitalWrite(LED_BOARD, HIGH);
		
  // Dispara dispositivo sonoro de teste (buzzer);        
  //shootBuzzer();

  ledShooting();
  
  triggerRelay(1);


  // TODO: Serial print
  Serial.println();
  Serial.print("DISPAROU action=true");
  Serial.print(" - Sensor: "); Serial.print(sensor);
  Serial.print(" - Info: "); Serial.print(local);


  //post data to realtime database
  postData(local);

}

