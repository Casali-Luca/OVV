#include <WiFiS3.h>
#include <dht11.h>
#define DHT11PIN 4
#define LIGHT_PIN A0  
#include "arduino_secrets.h"
dht11 DHT11;

/*
prima di procedere bisogna creare un file nella stessa directory del progetto con 
il nome arduino_secrets.h nella quale basterà aggiungere:
#define SSID "nomerete"
#define PSW "password"
#define IP "ip_api"
*/

const char ssid[] = SSID;  
const char pass[] = PSW; 

int pin = 8;
unsigned long duration;
unsigned long starttime;
unsigned long sampletime_ms = 2000;
unsigned long lowpulseoccupancy = 0;
float ratio = 0;
float concentration = 0;

WiFiClient client;
int status = WL_IDLE_STATUS;

int HTTP_PORT = 5500;
//porta aggiornata di flask + metodo post
String HTTP_METHOD = "POST";
//ip del server (WIP)
char HOST_NAME[] = IP;
//al termine di questa stringa aggiungere i valori rilevati, cambiati dal dizionario presente su github separati da una virgola
//sostituire con /insert_data/centrale/VALORE_cr1, VALORE_crN....
String PATH_NAME = "/insert_data/centrale/" ;
//chiave e valore del post
String queryString = "?ApplicationKey=oqwg-dash-jkbc-phuw-qgey-bhas-dapp";

void setup() {
  pinMode(LIGHT_PIN,  INPUT); 
  pinMode(8,INPUT);
  starttime = millis();
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

  duration = pulseIn(pin, LOW);
  lowpulseoccupancy = lowpulseoccupancy+duration;
  if ((millis()-starttime) >= sampletime_ms) //if the sampel time = = 30s
  {
    ratio = lowpulseoccupancy/(sampletime_ms*10.0);  
    concentration = 1.1*pow(ratio,3)-3.8*pow(ratio,2)+520*ratio+0.62;
    Serial.print("Concentration = ");
    Serial.print(concentration);
    Serial.println(" pcs/0.01cf");
    Serial.println("\n");
    lowpulseoccupancy = 0;
    starttime = millis();
  }

  float sensor_value = analogRead(LIGHT_PIN);
  float volts = sensor_value * 0.00488281;
  float microamps = volts * 100.0;
  float lux = microamps * 2.0;                 


  Serial.print ("Analog: ");
  Serial.print (sensor_value);
  Serial.print ("  Volts: ");
  Serial.print (volts);
  Serial.print ("  Lux: ");
  Serial.println (lux);

  int chk = DHT11.read(DHT11PIN);

  Serial.print("Humidity (%): ");
  Serial.println((float)DHT11.humidity, 2);

  Serial.print("Temperature  (C): ");
  Serial.println((float)DHT11.temperature, 2);
//inizio crypt data
char num[5];
char ris[5];
const char mappaCrittografia[] = {
  'L','H', 'k', '@', 'I', 't', '!', ']', 'S', 'd'
};


//lux
dtostrf(lux, 6, 2, num);
for(int i = 1, j = 0; i<=5; i++, j++){
  if(num[i]=='.'){
    ris[j] = 'p';
    }
  else{
    ris[j] =  mappaCrittografia[int(num[i])-48];
  }
}
String lux_c = ris;
lux_c = lux_c.substring(0,5);
//concentration
dtostrf(concentration, 6, 2, num);
for(int i = 1, j = 0; i<=5; i++, j++){
  if(num[i]=='.'){
    ris[j] = 'p';
    }
  else{
    ris[j] =  mappaCrittografia[int(num[i])-48];
  }
}
String concentration_c = ris;
concentration_c = concentration_c.substring(0,5);
//umidità
dtostrf((float)DHT11.humidity, 6, 2, num);
for(int i = 1, j = 0; i<=5; i++, j++){
  if(num[i]=='.'){
    ris[j] = 'p';
    }
  else{
    ris[j] =  mappaCrittografia[int(num[i])-48];
  }
}
String umidita_c = ris;
umidita_c = umidita_c.substring(0,5);
//temperatura
dtostrf((float)DHT11.temperature, 6, 2, num);
for(int i = 1, j = 0; i<=5; i++, j++){
  if(num[i]=='.'){
    ris[j] = 'p';
    }
  else{
    ris[j] =  mappaCrittografia[int(num[i])-48];
  }
}
String temperatura_c  = ris;
temperatura_c = temperatura_c.substring(0,5);
///insert_data/<station>/<temp>,<umidity>,<pm>,<luce>

queryString = temperatura_c+","+umidita_c+","+concentration_c+","+ lux_c+queryString;
Serial.println(queryString);

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
  delay(2000);
  queryString = "?ApplicationKey=oqwg-dash-jkbc-phuw-qgey-bhas-dapp";
}
