#include "Wire.h" //I2C library
#include "ADXL345.h"
#include "RHT03.h"
#include "ArduinoJson.h"

const int RHT03_DATA_PIN = 4; // RHT03 data pin
float temp;
float temparray[5];
int tempPin = A2; // Temperature input pin
int sampleTime = 1000; // 1 second dafault 
float occupancy;
float latestHumidity;
float latestTempF;
float severe;
  
RHT03 rht;
ADXL345 adxl;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(57600);
  Wire.begin();
  adxl.begin();
  rht.begin(RHT03_DATA_PIN);
  delay(1000);
  Serial.setTimeout(500);
}

void loop() {
  // put your main code here, to run repeatedly:
  int updateRet = rht.update();

  StaticJsonBuffer<200> jsonBuffer;
  String json = Serial.readStringUntil('}');
  if(json.length() == 0){
    json = "{}";
  }
  else{
    json = json + "}";
  }
  JsonObject& root = jsonBuffer.parseObject(json);
  if (!root.success()) {
    Serial.println("parseObject() failed");
    Serial.println(json);
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
  
  // If successful, the update() function will return 1.
  if (updateRet == 1)
  {
    // The humidity(), tempC(), and tempF() functions can be called -- after 
    // a successful update() -- to get the last humidity and temperature
    // value 
    latestHumidity = rht.humidity();
    latestTempF = rht.tempF();
  }
  else
  {
    // If the update failed, try delaying for RHT_READ_INTERVAL_MS ms before
    // trying again.
    delay(1000);
  }

  /*adxl.read();
  double xAxis = adxl.getX();
  double yAxis = adxl.getY();
  double zAxis = adxl.getZ();
  long   time  = millis(); 
*/
  int occu = analogRead(A1);
  if (occu>700){
    occupancy = 1;
  }
  else{
    occupancy = 0;
  }

  severe = (0.4682*(latestTempF)*(latestTempF) - 56.611*(latestTempF) + 2099.9)*occupancy;
  if(severe>1000){
    severe =1000;
  }
  root["TEMP"] = latestTempF;
  root["HUMIDITY"] = latestHumidity; 
  //root["XAXIS"] = xAxis;
  //root["YAXIS"] = yAxis;
  //root["ZAXIS"] = zAxis;
  root["OCCUPIED"] = occupancy;
  root["SEVERITY"] = severe;
  
  Serial.println();
//  if(Serial.available()){
//    root["MSG"] = Serial.readString();
//  }
  root.printTo(Serial); 
  delay(sampleTime);
}
