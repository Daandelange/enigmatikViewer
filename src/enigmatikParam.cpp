//
//  enigmatikParam.cpp
//  enigmatikViewer
//
//  Created by Daan de Lange on 30/4/14.
//  Copyright (c) 2014 __MyCompanyName__. All rights reserved.
//

#include "enigmatikParam.h"

template<typename ParameterType>
enigmatikParam<ParameterType>::~enigmatikParam(){
#ifdef USE_RPI_GPIO
	if(physicalPin>=0) wiringPiISRExtended( physicalPin, INT_EDGE_BOTH, NULL, NULL);
#endif
}

// MARK: Init
template<typename ParameterType>
void enigmatikParam<ParameterType>::setup() {
	physicalPin=-1;
}


// MARK: Loop
template<typename ParameterType>
void enigmatikParam<ParameterType>::update() {
		
}

template<typename ParameterType>
void enigmatikParam<ParameterType>::draw() {
	
}

template<typename ParameterType>
void enigmatikParam<ParameterType>::randomizeValue() {
	ParameterType tmp;
	tmp =  getRandomPossibleValue();
	ofParameter<ParameterType>::set(tmp);
	//set<ParameterType>( (ParameterType) tmp );
}

template<typename ParameterType>
void enigmatikParam<ParameterType>::randomizeTarget() {
	targetValue = getRandomPossibleValue();
}

template<typename ParameterType>
bool enigmatikParam<ParameterType>::linkWithGPIO(int pin){
	
	#ifdef USE_RPI_GPIO
		// link interrupt
		ofLogVerbose("enigmatikParam<>::linkWithGPIO") << "Linking a param with GPIO pin " << pin << "...";
		pinMode(pin, INPUT);
		pullUpDnControl(pin, PUD_UP);
		this->cast<bool>() = digitalRead(pin);
		physicalPin = pin;
		
		if( wiringPiISRExtended( pinA, INT_EDGE_BOTH, &myInterrupt, this) == -1 ){
			ofLogVerbose("enigmatikParam<>::linkWithGPIO") << "Failed! (pin " << pin << ")";
			// linking failed
			physicalPin = -1;
			return false;
		}
		else return true;
	#endif
	
	return false;
}

template<typename ParameterType>
void enigmatikParam<ParameterType>::myInterrupt(int pin, bool pinState){
	#ifdef USE_RPI_GPIO
		ofLogVerbose("enigmatikParam<>::myInterrupt") << "GPIO pin " << pin << "/" << physicalPin << " tiggered";
		
		if(pin != physicalPin) return;
		
		bool* value = this->cast<bool*>();
		*value = digitalRead(physicalPin);
		
	#endif
	return;
}


// - - - - - - - -
// enigmatikButton
enigmatikButton::enigmatikButton(){
	this->set("Switch", false);
	setMin(false);
	setMax(true);
	
	#ifdef ENIGMATIK_USE_VIRTUAL_BUTTONS
		randomizeValue(); // simulates a button state
	#endif
	
	randomizeTarget();
	
}

enigmatikButton::~enigmatikButton(){
	
}

void enigmatikButton::myInterrupt(int pin){
	#ifdef USE_RPI_GPIO
	
		ofLogVerbose("enigmatikParam<>::myInterrupt") << "GPIO pin " << pin << "/" << physicalPin << " tiggered";
	
		if(pin != physicalPin) return;
		
		// pushed ?
		if( digitalRead(physicalPin) ){
			// revert state
			bool* value = this->cast<bool*>();
			*value = !*value;
		}
	#endif
}


bool enigmatikButton::getRandomPossibleValue(){
	return (ofRandom(0.0f,1.0f)>.5f)?true:false;
}

float enigmatikButton::getDistFromTarget(){
	return ( this->cast<bool>() == true ) ? 1.f:0.f;
}


// - - - - - - - -
// enigmatikPushButton
enigmatikPushButton::enigmatikPushButton(){
	this->set("Pushbutton", 0);
	setMin(0);
	setMax(6); // enter whatever you want (changes difficulty)
	
#ifdef ENIGMATIK_USE_VIRTUAL_BUTTONS
	randomizeValue(); // simulates a button state
#endif
	randomizeTarget();
}

enigmatikPushButton::~enigmatikPushButton(){
	
}

int enigmatikPushButton::getRandomPossibleValue(){
	return round( ofRandom( getMin(), getMax() ));
}

float enigmatikPushButton::getDistFromTarget(){
	return abs(targetValue - this->cast<int>() );
}


// - - - - - - - -
// enigmatikThreePosSwitch
enigmatikThreePosSwitch::enigmatikThreePosSwitch(){
	this->set("ThreePosSwitch", 0);
	setMin(0);
	setMax(2);
	
#ifdef ENIGMATIK_USE_VIRTUAL_BUTTONS
	randomizeValue(); // simulates a button state
#endif
	randomizeTarget();
}

enigmatikThreePosSwitch::~enigmatikThreePosSwitch(){
	
}

int enigmatikThreePosSwitch::getRandomPossibleValue(){
	float r = ofRandom(0.0f,3.0f);
	return (r<1.0f)?0:((r<2)?1:2);
}

float enigmatikThreePosSwitch::getDistFromTarget(){
	return abs(targetValue - this->cast<int>() );
}


// - - - - - - - - - - - -
// enigmatikPotentiometer
enigmatikPotentiometer::enigmatikPotentiometer(){
	this->set("Potentiometer", 0);
	setMin(0);
	setMax(1023);
	
#ifdef ENIGMATIK_USE_VIRTUAL_BUTTONS
	randomizeValue(); // simulates a button state
#endif
	randomizeTarget();
}

enigmatikPotentiometer::~enigmatikPotentiometer(){
	
}

int enigmatikPotentiometer::getRandomPossibleValue(){
	return round( ofRandom( getMin(), getMax() ));
}

float enigmatikPotentiometer::getDistFromTarget(){
	int maxDist = getMax()/2; // loops so farthest away is
	int dist = abs(targetValue - this->cast<int>() );
	
	float ret = ofMap(dist, 0, maxDist, 0, 1);
	
	// snap on target ?
	float tolerance = .02f;
	if(ret<tolerance) ret=0;
	else ret=ofMap(ret-tolerance,-tolerance,1-tolerance,0,1);
	
	if(ret<0) ret=0;
	if(ret>1) ret=1-(ret-1);
	
	return ret;
}


// enigmatikRotaryEncoder
enigmatikRotaryEncoder::enigmatikRotaryEncoder(){
	this->set("Rotary Encoder", 0);
	setMin(-1023);
	setMax(1023);
	
#ifdef ENIGMATIK_USE_VIRTUAL_BUTTONS
	randomizeValue(); // simulates a button state
#endif
	randomizeTarget();
}

enigmatikRotaryEncoder::~enigmatikRotaryEncoder(){
	
}

void enigmatikRotaryEncoder::myInterrupt(int way, long total){
	this->cast<int>() = total;
	ofLogVerbose("enigmatikRotaryEncoder::myInterrupt") << "way=" << way << " - total=" << total;
}

int enigmatikRotaryEncoder::getRandomPossibleValue(){
	return round( ofRandom( getMin(), getMax() ));
}

float enigmatikRotaryEncoder::getDistFromTarget(){
	// return ofMap( abs(targetValue - this->cast<int>() ), 0, (getMax()-getMin())/2, 0.0f, 1.0f );
	
	int maxDist = (getMax()-getMin())/2; // loops so farthest away is
	int dist = abs(targetValue - this->cast<int>() );
	
	float ret = ofMap(dist, 0, maxDist, 0, 1);
	
	// snap on target ?
	float tolerance = .02f;
	if(ret<tolerance) ret=0;
	else ret=ofMap(ret-tolerance,-tolerance,1-tolerance,0,1);
	
	if(ret<0) ret=0;
	if(ret>1) ret=1-(ret-1);
	
	return ret;
}

#ifdef USE_RPI_GPIO
bool enigmatikRotaryEncoder::linkWithGPIOs(int _pin1, int _pin2){
	
	ofLogVerbose("enigmatikParam<>::linkWithGPIO") << "Linking a param with GPIO pins " << _pin1 << " and" << _pin2 << "...";
	
	physicalPin = _pin1;
	physicalPin2 = _pin2;
	
	rotaryEncoderInterface.setup(_pi1, _pin2, this, &myInterrupt);
	
	ofLogVerbose("enigmatikParam<>::linkWithGPIO") << "Done for " << _pin1 << " and" << _pin2 << "...";
	
	return true;
}
#endif