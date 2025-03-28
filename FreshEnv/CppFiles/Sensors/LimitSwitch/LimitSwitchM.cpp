// Limit Switches Class 
// Really don't know what the best way to make this class is
//Hardware Interrupts are limited to Pin 0-7
//Probably need 4 pins for limitSwitch
//Top and Bottom of the Motors, Arm Contacts, Door (Maybe)

//It is possible that we need to reset the interrupt flag, but it should already

#include <iostream>
#include <wiringPi.h>


class LimitSwitch {
	
	public:
		int Pin;
		void (*Action)(); //This is a pointer that points to another Function 
		//ie what will happen when the limit switch is triggered
		
		LimitSwitch(int pin, void (*action)()) {
			Pin = pin;
			Action = action;
		};
		
		
		// I don't know if I need to do this in each file or just the main file
		void Lib_Init(void) {
			if(wiringPiSetup() == -1) {
				//I want to use std::cerr, but not fully sure how it works
				std::cout << "WiringPiSetup() error on Pin: "; 
				std::cout << Pin;
				exit(1);
			};
		};
		
		void Pin_Init(void) {
			pinMode(Pin, INPUT);
			pullUpDnControl(Pin, PUD_UP);
			if( wiringPiISR(Pin, INT_EDGE_RISING, Action) < 0) {
				std::cerr << "Failed to set up ISR On PIN: " << Pin << std::endl;
			};
		};
		
		void Init(void) {
			Lib_Init();
			Pin_Init();
		};
		
};

void func() {
	std::cout << "this func works\n" << std::endl;
};

int main() {
	LimitSwitch S1(1, &func);
	S1.Init();
}
			





















