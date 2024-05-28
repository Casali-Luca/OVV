// https://wiki.dfrobot.com/CAN-BUS_Shield_V2__SKU__DFR0370_#target_4
 
  // demo: CAN-BUS Shield, send data
  #include <df_can.h>
  #include <SPI.h>
  #include <WiFiS3.h>
  #include <dht11.h>
  #define DHT11PIN 4
  #define LIGHT_PIN A0  
  #include "arduino_secrets.h"
  dht11 DHT11;

  //initializate WIFI
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
  //chiave e valore del post
  String queryString = "?ApplicationKey=oqwg-dash-jkbc-phuw-qgey-bhas-dapp";


  const int SPI_CS_PIN = 10;
  MCPCAN CAN(SPI_CS_PIN);

  void setup() {
    //setup for canbus
    pinMode(3, OUTPUT);
    Serial.begin(9600);    
    CAN.init();    
    CAN.init_Mask(MCP_RXM0, 0, 0x3ff); // there are 2 mask in mcp2515, you need to set both of them
    CAN.init_Mask(MCP_RXM1, 0, 0x3ff);
    /*
     * set filter, we can receive id from 0x04 ~ 0x09 except for 0x06
     * // there are 6 filter in mcp2515,so it can filter six id,i.e.0x04~0x09.
     */
    CAN.init_Filter(MCP_RXF0, 0, 0x01); //risponbde all'id=1
    //CAN.init_Filter(MCP_RXF1, 0, 0x05);                         // filter 1 for id = 0x05
    // CAN.init_Filter(MCP_RXF2, 0, 0x06);                         // filter 2 for id = 0x06
    //CAN.init_Filter(MCP_RXF3, 0, 0x07);                         // filter 3 for id = 0x07
    //CAN.init_Filter(MCP_RXF4, 0, 0x08);                         // filter 4 for id = 0x08
    //CAN.init_Filter(MCP_RXF5, 0, 0x09);  
    
    if(CAN_OK != CAN.begin(CAN_500KBPS)){
       Serial.println("CAN KO");
       abort();
    }    

    //setup for wifi
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

unsigned char len = 0;
unsigned char buf[8];
unsigned int lux;
unsigned int umi;
unsigned int tem;


void CentralInsert(){
  String PATH_NAME = "/insert_data/centrale/" ;
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


void loop(){

  //get data from canbus
  
  unsigned char data[1] = {'L'};

  CAN.sendMsgBuf(0x02,0,1, data);
  delay(1000);  
  if (CAN_MSGAVAIL == CAN.checkReceive()){
    CAN.readMsgBuf(&len, buf);    // read data,  len: data length, buf: data buf    
    lux = (byte)buf[1];
    lux = lux <<8;
    lux |= (byte)buf[0];
    Serial.print("luce: ");
    Serial.println(lux);
  }
  data[0] = {'U'};
  CAN.sendMsgBuf(0x02,0,1, data);
  delay(1000);
  if (CAN_MSGAVAIL == CAN.checkReceive()){
    CAN.readMsgBuf(&len, buf);    // read data,  len: data length, buf: data     
    umi = (byte)buf[1];
    umi = umi <<8;
    umi |= (byte)buf[0];
    Serial.print("umi: ");
    Serial.println(umi);
  }
  data[0] = {'T'};
  CAN.sendMsgBuf(0x02,0,1, data);
  delay(1000);
  if (CAN_MSGAVAIL == CAN.checkReceive()){
    CAN.readMsgBuf(&len, buf);    // read data,  len: data length, buf: data buf    
    tem = (byte)buf[1];
    tem = tem <<8;
    tem |= (byte)buf[0];
    Serial.print("tem: ");
    Serial.println(tem);
  }

  //Central Insert
  CentralInsert();


  //Left Insert
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
  
  dtostrf(umi, 6, 2, num);
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
  dtostrf(tem, 6, 2, num);
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

  queryString = temperatura_c+","+umidita_c+","+ lux_c+queryString;
  Serial.println(queryString);
  String PATH_NAME = "/insert_data/destra/" ;

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

