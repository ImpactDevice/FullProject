#include "LimitSwitch.h"
#include <iostream>
#include <wiringPi.h>
#include <functional>
		
		
		
		
		
	LimitSwitch::LimitSwitch(int pin, void (*func)()) :
		Pin(pin),
		Func(func)

	{
		Pin_Init();
	}
		
	

		
		
	// I don't know if I need to do this in each file or just the main file
	void LimitSwitch::Lib_Init(void) {
		if(wiringPiSetup() == -1) {
			//I want to use std::cerr, but not fully sure how it works
			std::cout << "WiringPiSetup() error on Pin: "; 
			std::cout << Pin;
			exit(1);
		};
	};
		
	void LimitSwitch::Pin_Init(void) {
		pinMode(Pin, INPUT);
		pullUpDnControl(Pin, PUD_UP);
		if( wiringPiISR(Pin, INT_EDGE_RISING, Func) < 0) {
			std::cerr << "Failed to set up ISR On PIN: " << Pin << std::endl;
		};
	};
		
	void LimitSwitch::Init(void) {
		//~ Lib_Init();
		Pin_Init();
	};
