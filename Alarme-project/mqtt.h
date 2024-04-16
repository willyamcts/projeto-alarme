#include <PubSubClient.h>
#include "credentials.h"


WiFiClient espClient;
PubSubClient clientMQTT(espClient);

const char* mqtt_topic = "/state";

/*
   Definitions functions
*/
bool subscribeTopics();
void callback(char* topic, byte* payload, unsigned int length);
void reconnect(const char* mqtt_user, const char* mqtt_password);


// TODO:: const in function???
void initMQTTConnection(const char* mqtt_server, unsigned int mqtt_port, String mqtt_user, const char* mqtt_password) {
  clientMQTT.setServer(mqtt_server, mqtt_port);

  // To receive messages from topic
  clientMQTT.setCallback(callback);
  
  // Subscribe in topics
  subscribeTopics();
}


// TODO: review
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived in topic [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  // Switch on the LED if an 1 was received as first character
  if ((char)payload[0] == '1') {
    digitalWrite(BUILTIN_LED, LOW);   // Turn the LED on (Note that LOW is the voltage level
    // but actually the LED is on; this is because
    // it is active low on the ESP-01)
  } else {
    digitalWrite(BUILTIN_LED, HIGH);  // Turn the LED off by making the voltage HIGH
  }
}

// Subscribe in all topics
bool subscribeTopics() {
  return clientMQTT.subscribe(mqtt_topic);
}



// Check connection with MQTT Broker
bool checkConnection(const char* mqtt_user, const char* mqtt_password) {
  bool value = true;
  if ( ! clientMQTT.connected() ) {
    value = true;
    reconnect(mqtt_user, mqtt_password);
  }
  return value;
}


void reconnect(const char* mqtt_user, const char* mqtt_password) {  
  while ( !clientMQTT.connected() ) {
    Serial.println();
    Serial.print("Attempting MQTT connection... ");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);

    //if (client.connect(clientId.c_str())) {
    if ( clientMQTT.connect(clientId.c_str(), mqtt_user, mqtt_password) ) {
      Serial.println("connected");
      // resubscribe
      subscribeTopics();

    } else {
      Serial.print("failed, rc=");
      Serial.print(clientMQTT.state());
      Serial.println(" try again in 3 seconds");
      delay(3000);
    }
  }
}


bool publishMessage(String message) {
  bool value = true;
  if ( ! clientMQTT.publish(mqtt_topic, message.c_str()) ) {
    Serial.printf("MQTT::publishMessage():: Fail post %s", message.c_str());
    value = false;
  }
  return value;
}