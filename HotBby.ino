#include "Wire.h" //I2C library
#include "ADXL345.h"
#include "RHT03.h"
#include "ArduinoJson.h"

const int RHT03_DATA_PIN = 4; // RHT03 data pin

float temp;
float temparray[5];
int tempPin = A2; // Temperature input pin
int sampleTime = 1000; // 1 second dafault 

RHT03 rht;
ADXL345 adxl;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(57600);
  Wire.begin();
  adxl.begin();
  rht.begin(RHT03_DATA_PIN);
  while (!Serial) continue;
  pinMode(LED_BUILTIN, OUTPUT);
  delay(1000);
}

void loop() {
  // put your main code here, to run repeatedly:
  int updateRet = rht.update();
  bool occupancy;
  float latestHumidity;
  float latestTempF;


  StaticJsonBuffer<200> jsonBuffer;
  char json[200] = "{}";
  JsonObject& root = jsonBuffer.parseObject(json);
  if (!root.success()) {
    Serial.println("parseObject() failed");
  }
  for(int i=4; i>0; i--){
    temparray[i] = temparray[i-1];
  }
  
  temp = analogRead(A2);
  temp = temp * 0.48828125;
  temp = temp *9 / 5;
  temp = temp + 32;
  temparray[0] = temp;
  temp = (temparray[0]*3+temparray[1]*4+temparray[2]*3+temparray[3]*2+temparray[4])/13;
  temparray[0] = temp;
  //root["TEMP"] = temp;
  
  // If successful, the update() function will return 1.
  if (updateRet == 1)
  {
    // The humidity(), tempC(), and tempF() functions can be called -- after 
    // a successful update() -- to get the last humidity and temperature
    // value 
    latestHumidity = rht.humidity();
    latestTempF = rht.tempF();
    //root["TEMP"] = latestTempF;
  }
  else
  {
    // If the update failed, try delaying for RHT_READ_INTERVAL_MS ms before
    // trying again.
    delay(1000);
    //return;
  }

  /*adxl.read();
  double xAxis = adxl.getX();
  double yAxis = adxl.getY();
  double zAxis = adxl.getZ();
  long   time  = millis(); 
*/
  int occu = analogRead(A1);
  if (occu>900)
  {
    occupancy = true;
    digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  }
  else
  {
    occupancy = false;
    digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
  }

  root["TEMP"] = latestTempF;
  root["HUMIDITY"] = latestHumidity; 
  //root["XAXIS"] = xAxis;
  //root["YAXIS"] = yAxis;
  //root["ZAXIS"] = zAxis;
  root["OCCUPIED"] = occupancy;
  
  Serial.println();
//  if(Serial.available()){
//    root["MSG"] = Serial.readString();
//  }
  root.printTo(Serial); 
  delay(sampleTime);
}
