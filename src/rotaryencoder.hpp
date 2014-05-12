#pragma once

#include <wiringPi.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <iostream>

template <class CBClass>
class re_decoder {

public:
	
	typedef void (CBClass::*re_decoderCB)(int, long);
	
	~re_decoder(){
		initialized = false;
	
		// unregister interupt events
		wiringPiISRExtended( pinA, INT_EDGE_BOTH, NULL, this);
		wiringPiISRExtended( pinB, INT_EDGE_BOTH, NULL, this);
	}
	
	void setup(int _pinA, int _pinB, CBClass *_instance, re_decoderCB _callback){
		pinA = _pinA;
		pinB = _pinB;
		lastPin = -1;
		callback = _callback;
		cb_instance = _instance;
		value = 0;
		
		pinMode(pinA, INPUT);
		pinMode(pinB, INPUT);
		
		pullUpDnControl( pinA, PUD_UP);
		pullUpDnControl( pinB, PUD_UP);
		
		wiringPiISRExtended( pinA, INT_EDGE_BOTH, &updateEncoderPin, this);
		wiringPiISRExtended( pinB, INT_EDGE_BOTH, &updateEncoderPin, this);
	 
		levA = digitalRead(pinA);
		levB = digitalRead(pinB);
	
		initialized = true;
	}

	void updateEncoderB();
	void updateEncoderA();
	
	static void updateEncoderPin(int pin, void* userdata){
		((re_decoder<CBClass>*)userdata)->updateEncoder(pin, digitalRead(pin) );
	}
	
	signed long value;

private:
	int lastPin;
	int levA;
	int levB;
	int pinA;
	int	pinB;
	re_decoderCB callback;
	CBClass* cb_instance;
	bool initialized;
	
	// freely inspired from void encoderPulse() - see: http://abyz.co.uk/rpi/pigpio/ex_rotary_encoder.html
	void updateEncoder(int gpio, int pinState){
		if(!initialized) return;
	
		if (gpio == pinA) levA = pinState; else levB = pinState;

		if (gpio != lastPin) {
			lastPin = gpio;

			if ((gpio == pinA) && (pinState == 1)){
				if (levB){
					value++;
					(cb_instance->*callback)(1, value);
				}
			}
			else if ((gpio == pinB) && (pinState == 1)){
				if (levA){
					value --;
					(cb_instance->*callback)(-1, value);
				}
			}
		}
	}
	
};
