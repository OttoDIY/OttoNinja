/*
 * ESP32PWM.cpp
 *
 *  Created on: Sep 22, 2018
 *      Author: hephaestus
 */

#include <ESP32PWM.h>
#include "esp32-hal-ledc.h"
// initialize the class variable ServoCount
int ESP32PWM::PWMCount = -1;              // the total number of attached servos
ESP32PWM * ESP32PWM::ChannelUsed[NUM_PWM]; // used to track whether a channel is in service

// The ChannelUsed array elements are 0 if never used, 1 if in use, and -1 if used and disposed
// (i.e., available for reuse)

ESP32PWM::ESP32PWM() {
	resolutionBits = 8;
	pwmChannel = -1;
	pin = -1;
}

ESP32PWM::~ESP32PWM() {
	// TODO Auto-generated destructor stub
}

void ESP32PWM::detach() {

	attachedState = false;

	//Serial.println("Detatching " + String(pwmChannel));
}

void ESP32PWM::attach(int p) {
	pin = p;

	getChannel();
	attachedState = true;
}


int ESP32PWM::getChannel() {
	if (PWMCount == -1) {
		for (int i = 0; i < NUM_PWM; i++)
			ChannelUsed[i] = NULL; // load invalid data into the storage array of pin mapping
		PWMCount = PWM_BASE_INDEX; // 0th channel does not work with the PWM system
	}
	if (pwmChannel < 0) {
		for (int i = PWM_BASE_INDEX; i < NUM_PWM; i++) {
			if (ChannelUsed[i] == NULL) {
				ChannelUsed[i] = this;
				pwmChannel = i;
				Serial.println("PWM requested on ledcwrite channel " + String(i) + " using timer "+String(getTimer()) );
				PWMCount++;
				return pwmChannel;
			}
		}
		Serial.println("ERROR All PWM channels requested! " + String(PWMCount));
	}
	return pwmChannel;
}


double ESP32PWM::setup(double freq, uint8_t resolution_bits) {
	checkFrequencyForSideEffects( freq);

	resolutionBits = resolution_bits;
	if (attached()) {
		detachPin(pin);
		double val = ledcSetup(getChannel(), freq, resolution_bits);
		attachPin(pin);
		return val;
	}
	return ledcSetup(getChannel(), freq, resolution_bits);
}

void ESP32PWM::writeScaled(float duty) {
	write(mapf(duty, 0.0, 1.0, 0, (float) ((1 << resolutionBits) - 1)));
}
void ESP32PWM::write(uint32_t duty) {
	ledcWrite(getChannel(), duty);
}
void ESP32PWM::adjustFrequency(double freq, float dutyScaled) {
	checkFrequencyForSideEffects( freq);
	if (attached()) {
		int APin = pin;
		detachPin(APin); // Remove the PWM during frequency adjust
		writeTone(freq); // update the time base of the PWM
		writeScaled(dutyScaled);
		attachPin(APin); // re-attach the pin after frequency adjust
	} else {
		writeTone(freq); // update the time base of the PWM
		writeScaled(dutyScaled);
	}
}
double ESP32PWM::writeTone(double freq) {
	resolutionBits = 10;
	return ledcWriteTone(getChannel(), freq);
}
double ESP32PWM::writeNote(note_t note, uint8_t octave) {
	resolutionBits = 10;
	return ledcWriteNote(getChannel(), note, octave);
}
uint32_t ESP32PWM::read() {
	return ledcRead(getChannel());
}
double ESP32PWM::readFreq() {
	return myFreq;
}
void ESP32PWM::attachPin(uint8_t pin) {
	if(hasPwm(pin)){
		attach(pin);
		ledcAttachPin(pin, getChannel());
	}else{
		Serial.println("ERROR PWM channel unavailible on pin requested! " + String(pin)+
				"\r\nPWM availible on: 2,4,5,12-19,21-23,25-27,32-33");

	}
}
void ESP32PWM::attachPin(uint8_t pin, double freq, uint8_t resolution_bits) {
	checkFrequencyForSideEffects( freq);
	if(hasPwm(pin))
		setup(freq, resolution_bits);
	attachPin(pin);
}

/* Side effects of frequency changes happen because of shared timers
 *
 * LEDC Chan to Group/Channel/Timer Mapping
** ledc: 0  => Group: 0, Channel: 0, Timer: 0
** ledc: 1  => Group: 0, Channel: 1, Timer: 0
** ledc: 2  => Group: 0, Channel: 2, Timer: 1
** ledc: 3  => Group: 0, Channel: 3, Timer: 1
** ledc: 4  => Group: 0, Channel: 4, Timer: 2
** ledc: 5  => Group: 0, Channel: 5, Timer: 2
** ledc: 6  => Group: 0, Channel: 6, Timer: 3
** ledc: 7  => Group: 0, Channel: 7, Timer: 3
** ledc: 8  => Group: 1, Channel: 0, Timer: 0
** ledc: 9  => Group: 1, Channel: 1, Timer: 0
** ledc: 10 => Group: 1, Channel: 2, Timer: 1
** ledc: 11 => Group: 1, Channel: 3, Timer: 1
** ledc: 12 => Group: 1, Channel: 4, Timer: 2
** ledc: 13 => Group: 1, Channel: 5, Timer: 2
** ledc: 14 => Group: 1, Channel: 6, Timer: 3
** ledc: 15 => Group: 1, Channel: 7, Timer: 3
*/

bool ESP32PWM::checkFrequencyForSideEffects(double freq){
	myFreq=freq;
	getChannel();
	for(int i=PWM_BASE_INDEX;i<PWMCount;i++){
		if(i==pwmChannel)
			continue;
		if(ChannelUsed[i]->getTimer()==getTimer()){
			double diff = abs(ChannelUsed[i]->myFreq-freq);
			if(abs(diff)>0.1){
				Serial.println("\tWARNING PWM channel "+String(pwmChannel)+" shares a timer with " + String(i)+"\n"
								"\tchanging the frequency to "+String(freq)+" Hz will ALSO change channel "+String(i)+" \n\tfrom its previous frequency of "+String(ChannelUsed[i]->myFreq)+" Hz\n"
								" ");
				ChannelUsed[i]->myFreq=freq;
			}
		}
	}
	return true;
}

void ESP32PWM::detachPin(uint8_t pin) {
	detach();
	return ledcDetachPin(pin);
}

ESP32PWM* pwmFactory(int pin) {
	for (int i = 0; i < NUM_PWM; i++)
		if (ESP32PWM::ChannelUsed[i] != NULL) {
			if (ESP32PWM::ChannelUsed[i]->getPin() == pin)
				return ESP32PWM::ChannelUsed[i];
		}
	return NULL;
}
