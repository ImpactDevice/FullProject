#include "StepperMotor.h"

#include <wiringPi.h>
#include <pthread.h>
#include <bits/stdc++.h>
#include <unistd.h> // For usleep fucntion
//~ #include <pigpio.h>
#include <iostream>



		
		//Constructor ie def __init__(self)
	StepperMotor::StepperMotor(int ClockPin, int DirPin, int ControlPin, float ClockFrequency, int MS1, int MS2) :
		Clock_Pin(ClockPin),
		Dir_Pin(DirPin),
		Control_Pin(ControlPin),
		Clock_hz(ClockFrequency),
		MS1_Pin(MS1),
		MS2_Pin(MS2)
	{
		//Other Setup Code can Go here
	}
		
	void StepperMotor::Init_Direction(void) {
		pinMode(Dir_Pin, OUTPUT);
		digitalWrite(Dir_Pin, Dir);
	};

	void StepperMotor::Init_Control(void) {
		pinMode(Control_Pin, OUTPUT);
		digitalWrite(Control_Pin, HIGH);
	};
		
	//Add A note to show which pins are allowed to be PWM_OUTPUT to clean up wiring	
	void StepperMotor::Init_Clock_PWM(void) {
		pinMode(Clock_Pin, PWM_OUTPUT);
		pwmSetMode(PWM_MODE_MS);
	
		//this is currently set for 2.5khz
		//Range
		pwmSetRange(108);
		//clock divisor
		pwmSetClock(100); 
		//Duty cycle part set to 1/2 of range
		pwmWrite(Clock_Pin, 54);
	};

	void StepperMotor::Init_Motor(void) {
		Init_Clock_PWM();
		Init_Direction();
		Init_Control();
	};
	
	void StepperMotor::Set_Corr_Factor(float Factor) {
		Corr_Fact = Factor;
	};
	
	//Need a way to deal with this function if a thread hasn't ever been
	//ran yet
	int StepperMotor::Is_Thread_Running(void) {
		return 0;
		int res = pthread_tryjoin_np(Thread, nullptr);
		if(res == 0) {
			return 0;
		} else if (res == EBUSY) {
			return 1;
		} else {
			std::cerr << "Error Checking Thread\n";
		}
		return 1;
	};
		
	void StepperMotor::Stop_Motor(void) {
		if(Is_Thread_Running()) {
			pthread_cancel(Thread); //Might need to check if the thread is already running before using cacnel()
		};
		pwmWrite(Clock_Pin, 0);
		delay(1); //make it easier to noticed for now
		//Also Should stop the PWM probably
	};
		
	void StepperMotor::Disengage_Motor(void) {
		//Don't know exactly what to do for this yet 
		digitalWrite(Control_Pin, LOW);
	};
		
	void StepperMotor::Engage_Motor(void) {
		digitalWrite(Control_Pin, HIGH);
	};
		
		//it might be better to use:
		//pthread_cancel(thread);
		//pthread_join(thread, nullptr);
		//to stop the motor
		
	void StepperMotor::Run_Motor_Forever(void) {
			//digitalWrite(Stepper_Dir_Pin, HIGH);
			//~ digitalWrite(Control_Pin, LOW);
			pwmWrite(Clock_Pin, 54);
	};
		
		//~ void* Run_Motor_Old(void* arg) {
			//~ //this step might be changed into this
			//~ //MyClass* instance = static_cast<MyClass*>(arg);
			//~ //StepperMotor* Motor = static_cast<StepperMotor*>(arg);
			//~ StepperData* data = static_cast<StepperData*>(arg);
			//~ float Period_Sec = 1.0/(Motor->Clock_hz);
			//~ int Period_us = (Motor->Steps)*round(Period_Sec*1000000)*(Motor->Step_Size);
			//~ digitalWrite(Stepper_Control_Pin, HIGH);
			//~ delayMicroseconds(Period_us);
			//~ digitalWrite(Stepper_Control_Pin, LOW);
			//~ std::cout << "Done With Motor Thread";
			//~ return nullptr;
		//~ }; 
		
	void* StepperMotor::Run_Motor_New() {
			//~ digitalWrite(Control_Pin, HIGH);
			pwmWrite(Clock_Pin, 54);
			delayMicroseconds(Run_Time_us);
			pwmWrite(Clock_Pin, 0);
			//~ digitalWrite(Control_Pin, LOW);
			// Might want to add this exit
			//pthread_exit(nullptr);
			return nullptr;
	};
			
		//Later On Convert this into 1 line for speed
	void StepperMotor::Set_Motor_Distance(float cm) {
			std::cout << Clock_hz;
			float cm_per_rev = Screw_Lead_in * (2.54);  // (inch/rev) * (cm/inch)
			//float cm_per_step = 1.0/200.0 * cm_per_rev; // (cm/rev) * (rev/step)
			//Correcting for what Sam Said:
			float cm_per_step = 1.0/120.0 * cm_per_rev;
			float pulse_per_cm = Step_Size / cm_per_step; // (Pulse/step) * (step/cm)
			float period_us = (1.0/Clock_hz)*1000000.0;
			float us_per_cm = pulse_per_cm * period_us; // (Pulse/cm) * (us/pulse)
			Run_Time_us = round(Corr_Fact * us_per_cm * cm);
			std::cout << "Run Time in us: " << Run_Time_us << std::endl;
			//Steps = Corr_Fact * Step_Size * Screw_Lead_in * (2.54) * cm / 200.0;
	};

	//CW: 1, CCW: 0 //CW: is Down
	void StepperMotor::Update_Motor_Direction(int dir) {
			Dir = dir;
			if(Dir == 0) {
				digitalWrite(Dir_Pin, 0);
			};
			if(Dir == 1) {
				digitalWrite(Dir_Pin, 1);
			};
	};

	void StepperMotor::Start_Motor_Thread(void) {
			pthread_t thread; //Still need to fix where this is defined
			if (pthread_create(&thread, nullptr, threadFunctionWrapper, this)) {
				std::cerr << "Error creating ThreadA" << std::endl;
				return ;
			};
			pthread_join(thread, nullptr);
	};
	//static void* StepperMotor::threadFunctionWrapper(void* arg)
	void* StepperMotor::threadFunctionWrapper(void* arg) {
			return static_cast<StepperMotor*>(arg)->Run_Motor_New();
	};
		
	void StepperMotor::Motor_Control(int Distance, int Direction) {
			Dir = Direction;
			//~ Update_Motor_Direction();
			pwmWrite(Clock_Pin, 0);
			if(Dir == 0) {
				digitalWrite(Dir_Pin, 0);
			};
			if(Dir == 1) {
				digitalWrite(Dir_Pin, 1);
			};
			Set_Motor_Distance(Distance);
			//~ Start_Motor_Thread();
			//Start_Motor_Thread();
			//Using Motor Thread Func works, but its easier to not use it
			Run_Motor_New();
	};
		
		//Not sure yet how exactly I should handle running threads outside of a function
		//Maybe the thread variable should be kept inside this class
		

		



//~ int main(void) {
	
	//~ if(wiringPiSetup() == -1) {
		//~ exit(1);
	//~ }
	
	//~ StepperMotor MotorA(26, 1, 0, 2000.0, 2, 3);
	//~ MotorA.Init_Motor();
	//~ MotorA.Motor_Control(5, 1);

	
	
	//~ return 0;
//~ };

















