#ifndef LIMITSWITCH_H
#define LIMITSWITCH_H
#include <functional>
#include "../MotorRepository/MotorCode/StepperMotor.h"

class LimitSwitch {
	
	private:
		int Pin;
		//~ std::function<void()> Func;
		void (*Func)(); //This is a pointer that points to another Function 
		//ie what will happen when the limit switch is triggered
		
		
		
	public:
		LimitSwitch(int pin, void (*action)());
		void Lib_Init(void);
		void Pin_Init(void);
		void Init(void);
		void Set_Func(std::function<void()> func);

		
};

#endif
