/******************************************************************************

*
******************************************************************************/

#include "BatReader9.h"

#if defined(ARDUINO) && ARDUINO >= 100
  #include "Arduino.h"
#else
  #include "WProgram.h"
#endif
void BatReader9::init(byte _BAT_PIN){
BAT_PIN = _BAT_PIN;

}



double BatReader9::readBatVoltage(void) {
	double readed = (double)(analogRead(BAT_PIN)*ANA_REF)/1024;
	if(readed > BAT_MAX) return BAT_MAX;
	else return readed;
}

double BatReader9::readBatPercent(void) {
	double value = (SLOPE*readBatVoltage()) - OFFSET;
	if(value < 0) return 0;
	else return value;
}

