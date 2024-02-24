#include <ESP8266WiFi.h>

void connectWifi(String wifiSSID, String wifiPassword) {
  // Let us connect to WiFi
  WiFi.begin(wifiSSID, wifiPassword);

  Serial.print("Connecting network");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected in \"" + WiFi.SSID() + "\" with IP Address: " + WiFi.localIP().toString() + " - DNS: " + WiFi.dnsIP(0).toString());
  
  // Reconnect AP case is it disconnected;
  WiFi.setAutoReconnect(true);
}