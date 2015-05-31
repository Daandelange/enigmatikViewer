//
//  enigmatikParam.h
//  enigmatikViewer
//
//  Created by Daan de Lange on 30/4/14.
//  Copyright (c) 2014 __MyCompanyName__. All rights reserved.
//

#pragma once

#include "ofMain.h"
#include "ofParameter.h"


#ifdef USE_RPI_GPIO
	#pragma message("Setting up GPIO for compilation")
	//#include <wiringPi> // todo:
	#include "wiringPiCppInterrupt.cpp"
	#include "rotaryencoder.hpp"
#else
	// uncomment for production (which uses physical buttons)
	#define ENIGMATIK_USE_VIRTUAL_BUTTONS
#endif

template<typename ParameterType>
class enigmatikParam : public ofParameter<ParameterType> {

public:
	
	~enigmatikParam();
	virtual void setup();
	void update();
	void draw();
	
	virtual void randomizeValue();
	virtual void randomizeTarget();
	virtual ParameterType getRandomPossibleValue()=0;
	virtual float getDistFromTarget() = 0; // 0 = match; 1=farthest away
	
	// physical button stuff
	virtual bool linkWithGPIO(int pin);
	virtual void myInterrupt(int pin, bool pinState);
	
	ParameterType targetValue;
	
protected:
	int physicalPin;
};


class enigmatikButton : public enigmatikParam<bool> {

public:
	enigmatikButton();
	~enigmatikButton();
	
	void myInterrupt(int pin);
	bool getRandomPossibleValue();
	float getDistFromTarget();
	
private:
	bool prevState;
};


class enigmatikPushButton : public enigmatikParam<int> {
	
public:
	enigmatikPushButton();
	~enigmatikPushButton();
	
	int getRandomPossibleValue();
	float getDistFromTarget();
};


class enigmatikThreePosSwitch : public enigmatikParam<int> {
	
public:
	enigmatikThreePosSwitch();
	~enigmatikThreePosSwitch();
	
	int getRandomPossibleValue();
	float getDistFromTarget();
};


class enigmatikPotentiometer : public enigmatikParam<int> {
	
public:
	enigmatikPotentiometer();
	~enigmatikPotentiometer();
	
	int getRandomPossibleValue();
	float getDistFromTarget();
};


class enigmatikRotaryEncoder : public enigmatikParam<int> {
	
public:
	enigmatikRotaryEncoder();
	~enigmatikRotaryEncoder();
	
	void myInterrupt(int way, long total);
	int getRandomPossibleValue();
	float getDistFromTarget();
};