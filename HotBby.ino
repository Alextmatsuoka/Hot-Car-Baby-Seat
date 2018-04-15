//Revision 2.0 Ready for demo 4 
//April 14 2018
#include <RFM69.h>
#include <SPI.h>
#include "RHT03.h"
#include "ArduinoJson.h"
// Addresses for this node. CHANGE THESE FOR EACH NODE!

#define NETWORKID   0   // Must be the same for all nodes
#define MYNODEID    2   // My node ID
#define TONODEID    1   // Destination node ID

#define FREQUENCY   RF69_915MHZ
#define ENCRYPT     true // Set to "true" to use encryption
#define ENCRYPTKEY  "STRING" // Use the same 16-byte key on all nodes

const int RHT03_DATA_PIN = 4; // RHT03 data pin
RFM69 radio;
RHT03 rht;
void(* resetFunc) (void) = 0; //declare reset function @ address 0

void setup()
{
  // Open a serial port so we can send keystrokes to the module:

  Serial.begin(9600);
  Serial.print("Node ");
  Serial.print(MYNODEID,DEC);
  Serial.println(" ready");  
  rht.begin(RHT03_DATA_PIN);

  radio.initialize(FREQUENCY, MYNODEID, NETWORKID);
  radio.setHighPower(); // Always use this for RFM69HCW

  if (ENCRYPT)
  radio.encrypt(ENCRYPTKEY);
}

int turn = 0;
void loop()
{
  // Set up a "buffer" for characters that we'll send:

  static char sendbuffer[62];
  static int sendlength = 0;

 
  if(turn){
  if (Serial.available() > 0){
 
    char input = Serial.read();
 
    if (input != '\r') // not a carriage return
      sendbuffer[sendlength++] = input;
 
    // If the input is a carriage return, or the buffer is full:
 
    if ((input == '\r') || (sendlength == 61)){ // CR or buffer full
   
      Serial.print("sending to node ");
      Serial.print(TONODEID, DEC);
      Serial.print(", message [");
      for (byte i = 0; i < sendlength; i++)
        Serial.print(sendbuffer[i]);
      Serial.println("]");
        radio.send(TONODEID, sendbuffer, sendlength);
      sendlength = 0; // reset the packet
      resetFunc();  //call reset
    }
  }
  turn = !turn;
  }else{
  turn = !turn;
  StaticJsonBuffer<200> jsonBuffer;
  String json = "{}";
  JsonObject& root = jsonBuffer.parseObject(json);
    int updateRet = rht.update();
    double temp = analogRead(A3);
    temp = temp * 0.48828125;
    temp = temp *9 / 5;
    temp = temp + 32;
    
    if (updateRet == 1){
      double latestHumidity = rht.humidity();
      double latestTempF = rht.tempF();
      double dew = pow((latestHumidity/100),(1/8))*(112+.9*(latestTempF))+(.1*(latestTempF)) - 112;
        root["TEMP"] = (temp);
        root["HUMIDITY"] = (latestHumidity);
        root["TEMP_OTHER"] = (latestTempF);
        root["DEW"] = (dew);
          int occu = analogRead(A1);
          int occupancy = 0;
          if (occu>700){
            occupancy = 1;
          }
          else{
            occupancy = 0;
          }
 
          int severe_t = (0.4682*(latestTempF)*(latestTempF) - 56.611*(latestTempF) + 2099.9)*occupancy;
          if(severe_t>1000){
            severe_t =1000;
          }
          root["SEVERITY_TEMP"] = severe_t;

          int severe_d = (0.2526*(dew)*(dew) - 23.64*(dew) + 1012.1) * occupancy;
          root["SEVERITY_DEWPOINT"] = severe_d;

          int max_severe;
          if(severe_t > severe_d){
            max_severe = severe_t;
          }
          else{
            max_severe = severe_d;
          }
          root["MAX_SEVERITY"] = max_severe;
        root.printTo(Serial);
        Serial.println();
    }else{
      delay(100);
    }

   
  }
}
