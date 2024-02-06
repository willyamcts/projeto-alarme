
#include <ESP8266WiFi.h>


void connectWifi(String wifiSSID, String wifiPassword) {
  // Let us connect to WiFi
  WiFi.begin(wifiSSID, wifiPassword);

  Serial.print("Connecting network");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.print("Connected in " + wifiSSID + ". IP Address: ");
  Serial.println(WiFi.localIP());

}