#include <FirebaseESP8266.h>
#include <jsmn.h>

#include "ntp_client.h"




// Define values in main class
//#define FIREBASE_HOST "a...firebaseio.com"
//#define FIREBASE_AUTH "16vw..."


FirebaseData firebaseData;

const String path = "/laT1udjSw8VORuaYuW15-Mec_Castro/";
const String fieldState = "state/on"; // should create field manually in db
//const String fieldLastTime = "state/lastChange";
//const String fieldLastLocale = "state/lastLocale";
const String fieldBootTime = "/bootHour";
const String fieldUptime = "/uptime";
const String fieldUptimeOn = "state/uptimeOn";

// fields registry trigger alarm
const String registry = "history/";
const String indexes = registry + "index";  // should create field manually in db, start value = 1
byte indexHistory;

const String fieldsLocale = registry + "locale";
const String fieldsTime = registry + "time";




/*
   Definitions functions
*/
bool postTimestamp(String path, String field);

bool postLocale(String path, String field, String locale);

void postUptime();
void postUptimeOn();



/*
   Init Firebase
*/

void initFirebase(String firebaseHost, String firebaseAuth) {
  Firebase.begin(firebaseHost, firebaseAuth);
  //initNTP();
}



/*
   Other functions
*/


bool checkDBAlarm() {

  if (Firebase.getInt(firebaseData, path + fieldState) ) {
    if  (firebaseData.dataType() == "int") {
      bool val = firebaseData.intData();

      Serial.println();
      Serial.print("Alarme foi: "); Serial.println(val);

      return val;
    }
  } else {
    return -1;
  }
}



void bootTime() {
  postTimestamp(path, fieldBootTime);
}



void postData(String locale) {

  if ( indexHistory == 0 ) {
    // Get index to registry history triggers;
    if (Firebase.getInt(firebaseData, path + indexes) ) {

      if  (firebaseData.dataType() == "int") {
        indexHistory = firebaseData.intData();
        Serial.print("Index do histórico: "); Serial.println(indexHistory);
      }

    }

    if ( indexHistory == 0 ) {
      indexHistory ++;
    }
    
  } else {
    
    indexHistory++;
    Firebase.setInt(firebaseData, path + indexes, indexHistory);
    Serial.print("Index incrementadoo.... "); Serial.println(indexHistory);
  }

  delay(100);

  if ( ! (postTimestamp(path, fieldsTime + indexHistory) && postLocale(path, fieldsLocale + indexHistory, locale)) ) {
    Serial.print("3RROR: ");
  }
  
/*
  if ( ! (postTimestamp(path, fieldLastTime) && postLocale(path, fieldLastLocale, locale)) ) {
    Serial.print("3RROR: ");
  }
*/

}



bool postTimestamp(String path, String fieldTimestamp) {

  if ( Firebase.setTimestamp(firebaseData, path + fieldTimestamp) ) {
    return true;
  }
}


bool postLocale(String path, String fieldLocale, String locale) {

  if (Firebase.setString(firebaseData, path + fieldLocale, locale) ) {
    return true;
  }
}


// @require campo "Uptime" e millis();
void postUptime() {

  if ( Firebase.setFloat(firebaseData, path + fieldUptime, millis()) ) {
    //return true;
  }
}



void postUptimeOn(unsigned long timeOn) {

  if ( Firebase.setFloat(firebaseData, path + fieldUptimeOn, timeOn) ) {
    //return true;
  }
}

