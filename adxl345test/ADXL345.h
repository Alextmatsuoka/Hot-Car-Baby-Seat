#ifndef ADXL345_h
#define ADXL345_h

#include "Arduino.h"
#include "Wire.h"

class ADXL345 {
	public:
		void begin();
		void read();
		double getX();
		double getY();
		double getZ();
	private:
		double _xAxis, _yAxis, _zAxis;

};

#endif