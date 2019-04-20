/*
 * ESP32PWM.h
 *
 *  Created on: Sep 22, 2018
 *      Author: hephaestus
 */

#ifndef LIBRARIES_ESP32SERVO_SRC_ESP32PWM_H_
#define LIBRARIES_ESP32SERVO_SRC_ESP32PWM_H_
#include "esp32-hal-ledc.h"
#define NUM_PWM 16
#define PWM_BASE_INDEX 0
#define USABLE_ESP32_PWM (NUM_PWM-PWM_BASE_INDEX)
#include <cstdint>

#include "Arduino.h"
class ESP32PWM {
private:
	void detach();
	void attach(int pin);
	int pwmChannel = 0;                         // channel number for this servo
	bool attachedState= false;
	int pin;
	uint8_t resolutionBits;
	double myFreq;
public:

	ESP32PWM();
	virtual ~ESP32PWM();

	//channel 0-15 resolution 1-16bits freq limits depend on resolution9
	double      setup( double freq, uint8_t resolution_bits);
	void        write( uint32_t duty);
	// Write a duty cycle to the PWM using a unit vector from 0.0-1.0
	void        writeScaled( float duty);
	double      writeTone( double freq);
	double      writeNote( note_t note, uint8_t octave);
	uint32_t    read();
	double      readFreq();
	void        attachPin(uint8_t pin);
	void        attachPin(uint8_t pin,double freq, uint8_t resolution_bits);
	void        detachPin(uint8_t pin);
	void adjustFrequency(double freq,float dutyScaled);
	static float mapf(float x, float in_min, float in_max, float out_min, float out_max) {
		return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
	}
	int getPin(){
		return pin;
	}
	int getChannel();
	bool attached(){
		return attachedState;
	}
	static bool hasPwm(int pin){
        if (
        		(pin ==2) || //1
        		(pin == 4) || //1
				(pin ==5) || //1
        		((pin >= 12) && (pin <= 19)) ||//8
				((pin >= 21) && (pin <= 23)) ||//3
                ((pin >= 25) && (pin <= 27)) || //3
				(pin == 32) || (pin == 33))//2
        	return true;
        return false;
	}
	static int channelsRemaining(){
		return NUM_PWM-PWMCount;
	}
	static int PWMCount;              // the total number of attached servos
	static ESP32PWM * ChannelUsed[NUM_PWM]; // used to track whether a channel is in service
	int getTimer(){
		return ((pwmChannel/2)%4);
	}
	bool checkFrequencyForSideEffects(double freq);

};

ESP32PWM* pwmFactory(int pin);

#endif /* LIBRARIES_ESP32SERVO_SRC_ESP32PWM_H_ */
