#include "Wire.h" //I2C library
#include "ADXL345.h"

ADXL345 adxl;

void setup() {
  Serial.begin(9600);
  Wire.begin();
  adxl.begin();
  Serial.println(">>>>>>>>>>>>>>>>>>>");
  Serial.println("Time\tXAxis\tYAxis\tZAxis");
}

void loop() {
  delay(100);
  adxl.read();
  double xAxis = adxl.getX();
  double yAxis = adxl.getY();
  double zAxis = adxl.getZ();
  long   time  = millis(); 
  Serial.println(); 
  Serial.print(time);
  Serial.print("\t");
  Serial.print(xAxis);
  Serial.print("\t");
  Serial.print(yAxis);
  Serial.print("\t");
  Serial.print(zAxis);
}
