/*
 * analogWrite.cpp
 *
 *  Created on: Sep 17, 2018
 *      Author: Harry-Laptop
 */

#include "analogWrite.h"
#include "ESP32PWM.h"

void analogWrite(uint8_t APin, uint16_t AValue) {
	if(APin== 25 ||APin==26){
		dacWrite(APin, AValue);
		return;
	}
	ESP32PWM* chan = pwmFactory(APin);
	if (chan == NULL) {
		chan = new ESP32PWM();
	}
	if(!chan->attached()){
		chan->setup(1000, 8); // 1KHz 8 bit
		chan->attachPin(APin); // This adds the PWM instance to the factory list
		//Serial.println("Attaching AnalogWrite : "+String(APin)+" on PWM "+String(chan->getChannel()));
	}
	if (AValue == 0) {
		if (chan->attached()) {
			chan->detachPin(APin);
			pinMode(APin, OUTPUT);
		}
		digitalWrite(APin, 0);
	} else if (AValue >= 255) {
		if (chan->attached()) {
			chan->detachPin(APin);
			pinMode(APin, OUTPUT);
		}
		digitalWrite(APin, 1);
	} else
		chan->write(AValue);
//    Serial.print( "ledcWrite: " ); Serial.print(  CESP32PWMPinMap[ APin ] - 1 ); Serial.print( " " ); Serial.println( AValue );
}
