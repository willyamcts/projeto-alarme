#include <Firebase_ESP_Client.h>
//#include <jsmn.h>
#include <addons/TokenHelper.h>

FirebaseAuth auth;
FirebaseConfig config;
FirebaseData firebaseData;

struct rtdb_structure {
  String root;
  const char* bootTime = "/bootTime";
  const char* state = "/state/on";
  const char* uptimeOn = "/state/uptimeOn";
  const char* shotsRecords = "/history/shots/";
};

rtdb_structure DB;


/*
   Definitions functions
*/
void setDefaultValuesRTDB();
void postTimestamp(String field);
bool postLocale(String field, String locale);


/*
 * Init Firebase
 */
void initFirebase(String firebaseHost, String firebaseKey, String firebaseUser, String firebaseUserPass) {
  config.host = firebaseHost;
  config.database_url = firebaseHost;
  config.api_key = firebaseKey;
  auth.user.email = firebaseUser;
  auth.user.password = firebaseUserPass;

  config.token_status_callback = tokenStatusCallback;
  firebaseData.setBSSLBufferSize(4096 /* Rx buffer size in bytes from 512 - 16384 */, 1024 /* Tx buffer size in bytes from 512 - 16384 */);

  Serial.printf("::initFirebase()::Firebase.begin() -> ");
  Firebase.begin(&config, &auth);

  if ( Firebase.ready() ) {
    DB.root = String((auth.token.uid).c_str()); // MB_String object type
    delay(10);
    Serial.printf("::initFirebase():::Firebase.ready() -> authenticated as %s = %s\n", firebaseUser.c_str(), DB.root.c_str());
  } else {
    Serial.printf("::initFirebase():::Firebase.ready() -> not authenticated, returned: %s\n", firebaseData.errorReason().c_str());
  }

  setDefaultValuesRTDB();
}

// Set default values for data in RTDB
void setDefaultValuesRTDB() {
  Firebase.RTDB.setInt(&firebaseData, DB.root + DB.uptimeOn, 0);
}


/*
 * Other functions
 */
void postBootTime() {
  postTimestamp(DB.bootTime);
}

void postUptimeOn() {
  postTimestamp(DB.uptimeOn);
}


bool checkState() {
  bool value = 0;
  if ( Firebase.RTDB.getInt(&firebaseData, (DB.root + DB.state).c_str()) ) {
    if (firebaseData.dataType() == "int") {
      value = firebaseData.intData();
      Serial.printf("::checkState() -> Alarm state = %d\n", value);
    }
  } else {
    Serial.printf("::checkState()::Firebase.RTDB.getInt(%s) error: %s", String(DB.root + DB.state).c_str(), firebaseData.errorReason().c_str());
  }
  return value;
}

void postData(char* locale) {
  Serial.printf("::Firebase.Timestamp = %d", firebaseData.to<int>());

  // TODO: https://github.com/mobizt/Firebase-ESP-Client?tab=readme-ov-file#store-data
  //if ( ! (postTimestamp(DB.root, DB.shotsRecords) && postLocale(DB.root, DB.shotsRecords + firebaseData.to<int>() + "/locale/", locale)) ) {
  postLocale(DB.shotsRecords + String(firebaseData.to<int>()) + "/locale/", locale);
}

void postTimestamp(String fieldTimestamp) {
  if ( Firebase.RTDB.setTimestamp(&firebaseData, DB.root + fieldTimestamp) ) {
    Serial.printf("::postTimestamp()::Firebase.RTDB.setTimestamp(%s)", fieldTimestamp.c_str());
  } else {
    Serial.printf("::postTimestamp()::Firebase.RTDB.setTimestamp in %s. error: %s", fieldTimestamp.c_str(), firebaseData.errorReason().c_str());
  }
}

bool postLocale(char* fieldLocale, char* locale) {
  bool value = true;
  if ( ! Firebase.RTDB.setString(&firebaseData, DB.root + fieldLocale, locale) ) {
    value = false;
    Serial.printf("POST fail %s: %s", DB.root + fieldLocale, firebaseData.errorReason().c_str());
  }
  return value;
}
