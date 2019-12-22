#include <FirebaseESP8266.h>
#include <FirebaseESP8266HTTPClient.h>
#include <FirebaseJson.h>
#include <jsmn.h>




// Define values in main class 
//#define FIREBASE_HOST "a...firebaseio.com"
//#define FIREBASE_AUTH "16vw..."


FirebaseData firebaseData;

const String path = "/state/laT1udjSw8VORuaYuW15/";
const String fieldState = "on";
const String fieldLastTime = "lastChange";
const String fieldLastLocale = "lastLocale";


/*
 * Definitions functions
 */
bool postTime(String path, String field);

bool postLocale(String path, String field, String locale);

/*
 * 
 */


void initFirebase(String firebaseHost, String firebaseAuth) {
  Firebase.begin(firebaseHost, firebaseAuth);
}


bool checkDBAlarm(String path, String field) {

  // TODO: Descomentar após testes iniciais
  //if (Firebase.getInt(firebaseData, path + field) {
  if (Firebase.getInt(firebaseData, "/state/laT1udjSw8VORuaYuW15/on")) {
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


void postData(String locale) {

  // TODO: Descomentar após testes iniciais
  //if (Firebase.setTimestamp(firebaseData, path + field) {
  if ( postTime(path, fieldLastTime) && postLocale(path, fieldLastLocale, locale) ) {
    
  } else {
    Serial.print("3RROR: ");  
  }
}



bool postTime(String path, String fieldTimestamp) {
  
  if ( Firebase.setTimestamp(firebaseData, path + fieldTimestamp) ) {
    return true;
  }
}

bool postLocale(String path, String fieldLocale, String locale) {

  if (Firebase.setString(firebaseData, path + fieldLocale, locale) ) {
    return true;
  }
}





