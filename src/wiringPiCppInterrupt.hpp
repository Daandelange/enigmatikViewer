#pragma once

#include <wiringPi.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <iostream>

template <class CBClass>
class wiringPiCppInterrupt {

public:
	
	typedef void (CBClass::*wpInterval)(int, bool);
	
	~re_decoder(){
		initialized = false;
	
		// unregister interupt events
		wiringPiISRExtended( pin, INT_EDGE_BOTH, NULL, this);
	}
	
	void setup(int _pin, CBClass *_instance, wpInterval _callback){
		pin = _pin;
		callback = _callback;
		cb_instance = _instance;
		onlyTriggerOnChange = false;
		minimumDelay = 30;
		
		// activate pin
		pinMode(pin, INPUT);
		pullUpDnControl( pin, PUD_UP);
	 
		lastState = digitalRead(pin);
		
		// wpISR() can return 0 if the interval hasn't triggered before timeOut.
		if(wiringPiISRExtended( pin, INT_EDGE_BOTH, &updateStatic, this) >= 0) initialized = true;
		else initialized = false;
	}
	
	static void updateStatic(int pin, void* userdata){
		((re_decoder<CBClass>*)userdata)->update(pin, digitalRead(pin) );
	}

	bool lastState; // caches the last pin state
	bool onlyTriggerOnChange; // can filter out some parasite calls
	unsigned int minimumDelay; // only call after a minimum delay in microseconds. 0=none

private:
	int pin;
	wpInterval callback;
	CBClass* cb_instance;
	bool initialized;
	std::clock_t lastStateChange;
	
	// calls the user-defined callback
	void update(int gpio, int pinState){
		if(!initialized || gpio != pin) return;

		// convert int state
		bool boolState = (pinState==1)?1:0;

		// filter out parasite calls ?
		if(onlyTriggerOnChange && boolState == lastState) return;

		// protect by time delay ?
		double duration;
		duration = ( std::clock() - lastStateChange ) / (double) CLOCKS_PER_SEC;
		if( minimumDelay > 0 && duration < minimumDelay ) return;

		lastState = boolState;
	
		else (cb_instance->*callback)(pin, boolState);
	}
	
};
