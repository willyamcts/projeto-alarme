
/*
#define SS_PIN 10
#define RST_PIN 9

*/

#include <SPI.h>
#include <MFRC522.h>


MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.

bool checkRFID() {

  bool state = false;
  
  // Procura por cartao RFID
  if ( ! mfrc522.PICC_IsNewCardPresent()) {
    return state;
  }
  
  // Seleciona o cartao RFID
  if ( ! mfrc522.PICC_ReadCardSerial()) {
    return;
  }
  
  //Mostra UID na serial
  Serial.print("UID da tag :");
  String conteudo= "";
  byte letra;
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
     Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
     Serial.print(mfrc522.uid.uidByte[i], HEX);
     conteudo.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     conteudo.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  Serial.println();
  conteudo.toUpperCase();

  if (conteudo.substring(1) == "00 15 1E A4") {
    /*
    Serial.print("Status: "); Serial.println(state);
    Serial.print("Conteudo: "); Serial.println(conteudo);
    */
    state = !state;

    //clean String
    conteudo.remove(1, 11);
    /*
  Serial.println();
  Serial.print("IN KEY OK, ESTADO= "); Serial.println(state); 
    Serial.print("Conteudo POS remove(): "); Serial.println(conteudo);
    */
  }
  
/*
  Serial.println();
  Serial.print("ESTADO in @return= ");
  Serial.println(state);
  */
  
  return state;
}
