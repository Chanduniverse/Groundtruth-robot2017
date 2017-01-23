/* Arduino ADNS2620 Library
 * Copyright 2010 SparkFun Electronic
 * Written by Ryan Owens
*/

#ifndef adns2620_dual_h
#define adns2620_dual_h

#include <avr/pgmspace.h>
#include "Arduino.h"

struct ADNS2620_Return {
   char  data[2];
};

class ADNS2620_DUAL
{
	public:
		ADNS2620_DUAL(int sda1, int sda2, int scl);
		void begin();
		void sync();
		struct ADNS2620_Return read(char address);
		void write(char address, char value);
	private:
		int _sda1;
		int _sda2;
		int _scl;
};

/* Register Map for the ADNS2620 Optical Mouse Sensor */
#define CONFIGURATION_REG   0x40
#define STATUS_REG          0x41
#define DELTA_Y_REG         0x42
#define DELTA_X_REG         0x43
#define SQUAL_REG           0x44
#define MAXIMUM_PIXEL_REG   0x45
#define MINIMUM_PIXEL_REG   0x46
#define PIXEL_SUM_REG       0x47
#define PIXEL_DATA_REG      0x48
#define SHUTTER_UPPER_REG   0x49
#define SHUTTER_LOWER_REG   0x4A
#define FRAME_PERIOD		0x4B

#endif