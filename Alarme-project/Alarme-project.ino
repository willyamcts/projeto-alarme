/*
 * Author: 
 *  Willyam Castro
 * 
 * Description: 
 *  This project is one basic residence alarm;
 * 
 * 
 * Components: 
 *  - NodeMCU ESP8266 12E
 *  - LED RGB
 *  - PIR Sensor
 *	- Magnetic sensor for doors and windows
 *  - Relay 5v
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
#define FIREBASE_HOST "<...>.firebaseio.com"
#define FIREBASE_APIKEY "AIk8rlh1haUitYi2Pgd7o0q25p0FGARJd7qgANW"
#define USER_EMAIL "<you account>"
#define USER_PASSWD "<password account>"

// Set data connection wireless;
#define WIFI_SSID "<wifi name>"
#define WIFI_PASSWORD "<wifi password>"



// Store alarm real status (on/off);
bool alarmOn;
// Store alarm status temporarily;
bool alarmOnTemp;
// Sensors status store;
bool action = false;

// TODO: A remover:: Intervalor de tempo para consultar o estado do alarme
const int timeCheck = 1000 * 15; // = 15 segundos


// Armazena hora do disparo (millis);
unsigned long currentMillisTrigger = 0; // 0
// "tempo de acionamento" do relé, em ms;
//	Conf: Altera somente o último número (minutos),
//    mantém o [60000]
const long triggerTime = 15000; //60000 * 2;


// TODO: A remover:: bloco todo a remover até "unsigned long checkDBMillis = millis();"
// intervalo de tempo para atualizar o tempo que o alarme está habilitado;
//  Conf: Altera somente o último número, mantém o [60000]
unsigned long uptimeOn = 60000 * 1;
// Armazena o tempo em que está habilitado;
unsigned long currentMillisOn = 0;
// Armazena o tempo decorrido até ativação do alarme, para ser descontado de 
//    @currentMillisOn
unsigned long discountMillisOn = 0;

unsigned long checkDBMillis = millis();



void setup() {
  Serial.begin(115200);
  
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
   * Define states
   */
  digitalWrite(LED_BOARD, LOW);
  digitalWrite(RELAY, LOW);


/*  
  // DHT
  pinMode(A0, INPUT_PULLUP);
  delayMicroseconds(100);
*/

/*
  // Start RFID RC-522
  SPI.begin();      // Inicia  SPI bus
  mfrc522.PCD_Init();   // Inicia MFRC522
*/

  ledConnectingWifi(true);
  connectWifi(WIFI_SSID, WIFI_PASSWORD);

// TODO: testar rede para trocar cor: "conectado", "sem internet"
  ledConnectingWifi(false);
  // TODO: remover::: redundante ao da func wifi.h
  //Serial.printf("Connected WIFI_SSID: %s", WiFi.localIP());


/*
  1. inicializa connect RTDB
  2. POST bootTime;
  3. Consulta db para set state local
  4. if state = on -> set(uptimeOn) que deve ser o timestamp -- o calculo do tempo deve ser feito no device final,
     a cada view no device fazer uma request checando se realmente esta ativo antes de apresentar como ativo;
  5. subscribe MQTT -> topic /state ou /state_$UID ou /$UID/state
  -- print SerialMonitor valores coletado e atribuidos em RTDB
  N+1. function loop();
  *
  *
  *
  *
  */

/*
  initFirebase(FIREBASE_HOST, FIREBASE_APIKEY);
// TODO: verificar para usar NEW Firebase.Stream
//dbStream();
*/

  // start firebase connection
  initFirebase(FIREBASE_HOST, FIREBASE_APIKEY, USER_EMAIL, USER_PASSWD);

  postBootTime(); // Timestamp

  alarmOn = checkState();
  if ( alarmOn ) {
    postUptimeOn(); // Timestamp
  }


  // DHT
  //initDHT();
}



void loop() {
  //digitalWrite(LED_BOARD, HIGH);

  /*
  1. MQTT loop() para receber alterações? modifica valores como state, uptimeOn in RTDB 
     -- LED indicativo e uptimeOn deve ser alterado se state mudar
  2. Salva dados pertinentes no RTDB
  3. mantem "if ( alarmOn ) {": remover postUptimeOn
  4. não alterar checkStatusModules() e shoot()
  *
  *
  *
  *
  */

  //TODO: a remover::: funcao inteira
  // Check db in interval defined;
  if ( millis() > (checkDBMillis + timeCheck) ) {
    checkDBMillis = millis();

/*
// TODO: NEW
    if ( Firebase.available() ) {
      Serial.print("NOVA CONSULTA: "); Serial.println(checkDBAlarm("", ""));
    }
*/
    
    //alarmOnTemp = checkDBAlarm();

    //postUptime();

    // TODO: APAGAR / TESTE
    alarmOn=0;
    alarmOnTemp = 0;

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
        //postUptimeOn(currentMillisOn);
        
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

    // TODO: a remover::: ao inves disso fazer: se houver interação no app, faz um publish que solicita info ao ESP e o mesmo 
    //    atualiza o RTDB com o uptimeOn como forma de validar que realmente está ativo
      // @millis()
      //Atualiza tempo do alarme ativo @uptimeOn
    if ( millis() > ((currentMillisOn+discountMillisOn) + uptimeOn) ) {    
      currentMillisOn = millis() - discountMillisOn;
      //postUptimeOn(currentMillisOn);
      
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

  // TODO: definir arrays no inicio para alterar apenas o bloco inicial e nao precisar alterar esta função a cada alteração
  //    de módulo
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

  // For debug
  Serial.printf("Shooting...\n - Sensor: %s\n - Info: %s", sensor, local);

  //post data to realtime database
  //postData(local);

}