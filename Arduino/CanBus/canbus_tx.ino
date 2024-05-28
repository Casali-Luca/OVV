// https://wiki.dfrobot.com/CAN-BUS_Shield_V2__SKU__DFR0370_#target_4
// demo: CAN-BUS Shield, send data
  #include <df_can.h>
  #include <dht11.h>
  #include <SPI.h>

  #define LIGHT_PIN A0  
  #define DHT11PIN 4  

  dht11 DHT11;

  const int SPI_CS_PIN = 10;
  MCPCAN CAN(SPI_CS_PIN);

  void setup() {
    Serial.begin(9600);    
    CAN.init();
     CAN.init_Mask(MCP_RXM0, 0, 0x3ff); // there are 2 mask in mcp2515, you need to set both of them
    CAN.init_Mask(MCP_RXM1, 0, 0x3ff);
    /*
     * set filter, we can receive id from 0x04 ~ 0x09 except for 0x06
     * // there are 6 filter in mcp2515,so it can filter six id,i.e.0x04~0x09.
     */
    CAN.init_Filter(MCP_RXF0, 0, 0x02);

    if(CAN_OK != CAN.begin(CAN_500KBPS)){
       Serial.println("CAN KO");
       abort();
    }    
}

char statoled = 0;
//unsigned char data[8] = {'D', 'F', 'R', 'O', 'B', 'O', 'T', '!'};
unsigned char data[6] = {0,0,0,0,0,0};
unsigned char len = 0;
unsigned char buf[8];


void loop() {
  if (CAN_MSGAVAIL == CAN.checkReceive()){
    CAN.readMsgBuf(&len, buf);    // read data,  len: data length, buf: data buf    
    for (int i = 0; i<len; i++) {
       Serial.write(buf[i]);
       Serial.print("\t");
    }
    switch(buf[0]){
      case 'L':{
        float sensor_value = analogRead(LIGHT_PIN);
        float volts = sensor_value * 0.00488281;
        float microamps = volts * 100.0;
        float lux = microamps * 2.0;

        Serial.println(lux);

        unsigned int x;
        if(lux<10){
            lux = lux*10;
        }
        x = lux;
        
        //Serial.println(x);

        data[0] = (char)((x & 0x000FF));
        data[1] = (char)((x & 0xFF00) >> 8);
        CAN.sendMsgBuf(0x01, 0, 2, data);

        break;
      }
      case 'U':{
        int chk = DHT11.read(DHT11PIN);

        unsigned int umi = (int)DHT11.humidity;
        Serial.println(umi);
        data[0] = (char)(umi & 0x0000FF);
        data[1] = (char)((umi & 0x00FF00) >> 8);
        
        CAN.sendMsgBuf(0x01, 0, 2, data);
        break;
      } 
      case 'T':{
        int chk = DHT11.read(DHT11PIN);
        unsigned int temp = (int)DHT11.temperature;
        Serial.println(temp);
        data[0] = (char)(temp & 0x0000FF);
        data[1] = (char)((temp & 0x00FF00) >> 8);
        
        CAN.sendMsgBuf(0x01, 0, 2, data);
        break;
      }
      default:
        break;
    }
  }






  // send data:  id = 0x06, standrad flame, data len = 8, data: data buf
  
  /*
  data[0] = (char)((x & 0x0000FF));
  data[1] = (char)((x & 0x00FF00) >> 8);
  data[2] = (char)(umi & 0x0000FF);
  data[3] = (char)((umi & 0x00FF00) >> 8);
  data[4] = (char)(temp & 0x0000FF);
  data[5] = (char)((temp & 0x00FF00) >> 8);
*/

}
  