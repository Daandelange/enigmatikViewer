#include "wiringPiCppInterrupt.hpp"
// note: all functions are defined in the header file because they won't link otherwise

// needs an altered version of wiringPi
// https://github.com/Daandelange/WiringPi

// before using wiringPiCppInterrupt<MyCallbackClass>::MyInstance.setup(int pin, (void*)(callback)(int, ); setup(); make sur you called wiringPiSetup();
// example: ( /!\ brain compiled )
/*

#include <wiringPi>
#include <wiringPiInterrupt.hpp>

MyButton button(5); // listen to GPIO 5

void main(){

	// here you can access
	// button->value;

	return 0;
}

class MyCounter {

public: 
	MyCounter(unsigned int pin){
		
		// listen to pin
		myInterrupt.setup(pin, this, &MyCounter::interruptCallBack );
	}
	
	// structure must be the same
	void interruptCallBack(int pin, bool pinState){
		value ++; // each time the pin (connected to a push button) is trigered, incremment value
	}

	wiringPiCppInterrupt<MyCounter> myInterrupt;

	int value;
}

*/