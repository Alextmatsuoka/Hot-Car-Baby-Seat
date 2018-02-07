#include "Arduino.h"
#include "Wire.h"
#include "ADXL345.h"


void ADXL345::begin(){
   _xAxis = 0;
   _yAxis = 0;
   _zAxis = 0;
   Wire.beginTransmission(0x53);//Accel Address
   Wire.write(0x2D);
   Wire.write(0x08);//Configures the POWER_CTL to start measuring
   Wire.endTransmission();
}

void ADXL345::read(){
  Wire.beginTransmission(0x53);
  Wire.write(0x32);
  Wire.requestFrom(0x53, 6);
  int buffer[6];
  int bufferNumber = 0;
  
  while(Wire.available()){
    buffer[bufferNumber] = Wire.read();
    bufferNumber++;
  }
  Wire.endTransmission();
  
  _xAxis = ((int)((buffer[1] << 8) & 0xff00) | (buffer[0] & 0xff));
  _yAxis = ((int)((buffer[3] << 8) & 0xff00) | (buffer[2] & 0xff));
  _zAxis = ((int)((buffer[5] << 8) & 0xff00) | (buffer[4] & 0xff)); 
}

double ADXL345::getX(){
  return _xAxis * 0.004; 
}

double ADXL345::getY(){
  return _yAxis * 0.004; 
}

double ADXL345::getZ(){
  return _zAxis * 0.004; 
}