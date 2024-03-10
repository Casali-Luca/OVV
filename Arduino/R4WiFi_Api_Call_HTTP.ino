#include <WiFiS3.h>

/*
prima di procedere bisogna creare un file nella stessa directory del progetto con 
il nome arduino_secrets.h nella quale basterà aggiungere:

#DEFINE SECRET_SSID "nomewifi"
#DEFINE SECRET_PASS "passwifi"
*/
#include "arduino_secrets.h"


const char ssid[] = SECRET_SSID;  
const char pass[] = SECRET_PASS; 

WiFiClient client;
int status = WL_IDLE_STATUS;

int HTTP_PORT = 5500;
//porta aggiornata di flask + metodo post
String HTTP_METHOD = "POST";
//ip del server (WIP)
char HOST_NAME[] = "192.168.1.67";
//al termine di questa stringa aggiungere i valori rilevati, cambiati dal dizionario presente su github separati da una virgola
//sostituire con /insert_data/centrale/VALORE_cr1, VALORE_crN....
String PATH_NAME = "/print_all/centrale";
//chiave e valore del post
String queryString = "?ApplicationKey=oqwg-dash-jkbc-phuw-qgey-bhas-dapp";

void setup() {
  Serial.begin(9600);
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed!");
    while (true);
  }

  //controllo firmware
  String fv = WiFi.firmwareVersion();
  if (fv < WIFI_FIRMWARE_LATEST_VERSION) {
    Serial.println("Please upgrade the firmware");
  }

  //connessione
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    status = WiFi.begin(ssid, pass);
    delay(10000);
  }

  //ip scheda arduino
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  
}

void loop() {

  if (client.connect(HOST_NAME, HTTP_PORT)) {
    Serial.println("Connected to server");
    client.println(HTTP_METHOD + " " + PATH_NAME + queryString + " HTTP/1.1");
    client.println("Host: " + String(HOST_NAME));
    client.println("Connection: close");
    client.println();

    // client.println(queryString);

    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.print(c);
      }
    }
    client.stop();
    Serial.println();
    Serial.println("disconnesso");
  } else { 
    Serial.println("connection failed");
  }
  delay(90000);
}
