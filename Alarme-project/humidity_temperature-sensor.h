#include <DHT.h>
#include "pins.h"

// set model sensor DHT
# define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

/*
 * Init DHT
 */
void initDHT() {
  dht.begin();
  Serial.println("DHT iniciado...");  
}



/*
 * Other functions
 */

void readDHT() {
  // A leitura da temperatura e umidade pode levar 250ms!
  // O atraso do sensor pode chegar a 2 segundos.
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  delay(1500);

// TODO: Teste
    Serial.print(h); Serial.print(" - "); Serial.println(t);
    
  // test return is valid
  if (isnan(t) || isnan(h)) {
    Serial.println("Failed to read from DHT");
  } else {
    char sbuf[100]; // <~~~~ same size
    sprintf(sbuf, "Cels.:%f   Fahrh:%f", h,t);  // <~~~~ same line 
    Serial.println(sbuf);
    Serial.print("Umidade: ");
    Serial.print(h);
    Serial.print(" %t");
    Serial.print("Temperatura: ");
    Serial.print(t);
    Serial.println(" *C");
  }
}

