
#include <ESP8266WiFi.h>


void connectWifi(String wifiSSID, String wifiPassword) {
  // Let us connect to WiFi
  WiFi.begin(wifiSSID, wifiPassword);

 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  //Serial.println(".......");
  Serial.print(" WiFi Connected....IP Address: ");
  Serial.println(WiFi.localIP());

}
