/*
  * Instruções e fonte: https://www.instructables.com/id/Post-to-Google-Docs-with-Arduino/
  * Fonte modificado para atender o módulo Ethernet W5500, necessário lib Ethernet2
*/

#include <Ethernet2.h>

///////////////////////////////
///      EDIT THIS STUFF     //
///////////////////////////////

byte mac[] = {0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF };  //Replace with your Ethernet shield MAC
byte ip[] = {192,168,15,50};     // Your Arduino device IP address

//String devid = "v53C4CA92F877E49";  // THIS IS THE DEVICE ID FROM PUSHINGBOX
String devid = "v53C4CA92F877E49";  // THIS IS THE DEVICE ID FROM PUSHINGBOX

int del=300;  // Amount of seconds delay between posting to google docs.

///////////////////////////////
//       DONE EDITING        //
///////////////////////////////

// Definitions as server 
EthernetServer server(80);
String readString;
 
// Definitions as client
EthernetClient client;
char postmsg[100];
char serverAPI[] = "api.pushingbox.com";





int initServer(bool alarmOn) {

  int onTemp = alarmOn;
  
  EthernetClient clientWeb = server.available();

  // true in case request page and interating
  if (clientWeb) {

    // TODO
    Serial.println("Mod. ethernet");
    while (clientWeb.connected()) {

      // TODO
      Serial.println();
      Serial.print(" ---- Cliente conectado");
      if (clientWeb.available()) {
        char c = clientWeb.read();
  
        if (readString.length() < 100) {
          readString += c;             
        }
 
        if (c == '\n') {
          clientWeb.println("HTTP/1.1 200 OK");
          clientWeb.println("Content-Type: text/html");
          clientWeb.println();
           
          clientWeb.println("<HTML>");
          clientWeb.println("<BODY>");
          clientWeb.println("<H1>  CONTROLE  </H1>");
          clientWeb.println("<hr />");
          clientWeb.println("<br />");
           
          clientWeb.println("<a href=\"/?on\">Ligar</a>");
          clientWeb.println("<br />");
          clientWeb.println("<a href=\"/?off\">Desligar</a><br />");
          clientWeb.println("<br />"); clientWeb.println("<br />");

          delay(1);
          if ( alarmOn ) {
            clientWeb.println("<b> ON </b>");
          } else {
            clientWeb.println("<b> OFF </b>");
          }
           
          clientWeb.println("</BODY>");
          clientWeb.println("</HTML>");
           
          delay(1);
          clientWeb.stop();
           
          if(readString.indexOf("?on") > 0) {
            onTemp = 1; Serial.println(); Serial.println("* Alarme ativado via web...");
          } else {
            if(readString.indexOf("?off") > 0) {
              onTemp = 0; Serial.println(); Serial.println("* Alarme desativado via web");
            } 
          }
          
          readString="";    
        }
      }

      // stop while if change value in alarmOn
      if ( onTemp != alarmOn ) {
        
        // TODO
        Serial.print("Para a verificação de cliente: ");

        
        break;;
      }
    }
  }

  return onTemp;
}




void postForm(String sensor, String info) {
  
  // Post to Google Form.............................................
  if (client.connect(serverAPI, 80)) {

    Serial.println("connected");
    sprintf(postmsg,"GET /pushingbox?devid=%s&status=%s-%s HTTP/1.1", devid.c_str(), sensor.c_str(), info.c_str());  // NOTE** In this line of code you can see where the temperature value is 
      // inserted into the wed address. It follows 'status=' Change that value to whatever you want to post.

    
    client.println(postmsg);
  delay(500);
    client.println("Host: api.pushingbox.com");
    client.println("Connection: close");
    client.println();

    Serial.println(postmsg);
    Serial.println("Host: api.pushingbox.com");
    Serial.println("Connection: close");
 
     delay(1000);
     client.stop();
  }
  delay(1000);
  
  if (!client.connected()) {
    Serial.println();
    Serial.println("disconnecting.");
    client.stop();

    return;
  }

 
}
