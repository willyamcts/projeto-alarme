#include <FirebaseESP8266.h>
#include <FirebaseESP8266HTTPClient.h>
#include <FirebaseJson.h>
#include <jsmn.h>




// Define values in main class 
//#define FIREBASE_HOST "a...firebaseio.com"
//#define FIREBASE_AUTH "16vw..."


FirebaseData firebaseData;

bool postTime(String path, String field);

bool postLocale(String path, String field, String locale);


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


void postData(String path, String field, String locale) {

  // TODO: Descomentar após testes iniciais
  //if (Firebase.setTimestamp(firebaseData, path + field) {
  if ( postTime("", "") && postLocale("", "", locale) ) {
    Serial.println("Timestamp atribuido ao banco");


  } else {
    
    Serial.print("3RROR: ");  
  }
}


bool postTime(String path, String field) {
  //if (Firebase.setTimestamp(firebaseData, path + field) {
  Firebase.setTimestamp(firebaseData, "/state/laT1udjSw8VORuaYuW15/lastChange");
}

bool postLocale(String path, String field, String locale) {
  //if (Firebase.setString(firebaseData, path + field) {
    Firebase.setString(firebaseData, "/lastLocale", locale);
}





