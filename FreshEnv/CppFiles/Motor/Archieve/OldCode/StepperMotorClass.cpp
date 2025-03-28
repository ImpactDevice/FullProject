#include <wiringPi.h>
#include <pthread.h>
#include <bits/stdc++.h>
#include <unistd.h> // For usleep fucntion
//~ #include <pigpio.h>
#include <iostream>


class StepperMotor {

	public:
		int Clock_Pin;
		int Dir_Pin;
		int Control_Pin;
		int MS1_Pin;
		int MS2_Pin;
		int Clock_hz = 2500;
		int Dir = 0;
		int Step_Size = 8.0;
		float Corr_Fact = 1.154;
		int Run_Time_us;
		float Screw_Lead_in = 0.333;
		static pthread_t Thread;
		
		//thread var here?
		
		int Steps;
		
		//Constructor ie def __init__(self)
		StepperMotor(int ClockPin, int DirPin, int ControlPin, int ClockFrequency, int MS1, int MS2) {
			Clock_Pin = ClockPin;
			Dir_Pin = DirPin;
			Control_Pin = ControlPin;
			Clock_hz = ClockFrequency;
			MS1_Pin = MS1;
			MS2_Pin = MS2;
		}
		
		void Init_Direction(void) {
			pinMode(Dir_Pin, OUTPUT);
		};

		void Init_Control(void) {
			pinMode(Control_Pin, OUTPUT);
			digitalWrite(Control_Pin, HIGH);
		};
		
		void Init_Clock_PWM(void) {
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

		void Init_Motor(void) {
			Init_Clock_PWM();
			Init_Direction();
			Init_Control();
		};
		
		void Stop_Motor(void) {
			digitalWrite(Control_Pin, LOW);
			//Also Should stop the PWM probably
		};
		
		void Disengage_Motor(void) {
			//Don't know exactly what to do for this yet 
			digitalWrite(Control_Pin, LOW);
		};
		
		void Engage_Motor(void) {
			digitalWrite(Control_Pin, HIGH);
		};
		
		//it might be better to use:
		//pthread_cancel(thread);
		//pthread_join(thread, nullptr);
		//to stop the motor
		
		void Run_Motor_Forever(void) {
			//digitalWrite(Stepper_Dir_Pin, HIGH);
			digitalWrite(Control_Pin, LOW);
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
		
		void* Run_Motor_New() {
			digitalWrite(Control_Pin, HIGH);
			pwmWrite(Clock_Pin, 54);
			delayMicroseconds(Run_Time_us);
			pwmWrite(Clock_Pin, 0);
			//~ digitalWrite(Control_Pin, LOW);
			return nullptr;
		};
			
		//Later On Convert this into 1 line for speed
		void Set_Motor_Distance(int cm) {
			float cm_per_rev = Screw_Lead_in * (2.54);  // (inch/rev) * (cm/inch)
			float cm_per_step = 1.0/200.0 * cm_per_rev; // (cm/rev) * (rev/step)
			float pulse_per_cm = Step_Size / cm_per_step; // (Pulse/step) * (step/cm)
			float period_us = (1.0/Clock_hz)*1000000.0;
			float us_per_cm = pulse_per_cm * period_us; // (Pulse/cm) * (us/pulse)
			Run_Time_us = round(Corr_Fact * us_per_cm * cm);
			std::cout << "Run Time in us\n";
			std::cout << Run_Time_us;
			std::cout << "\n";
			std::cout << "cm per rev\n";
			std::cout << cm_per_rev;
			std::cout << "\n us_per_cm\n";
			std::cout << us_per_cm;
			std::cout << "\n";
			//Steps = Corr_Fact * Step_Size * Screw_Lead_in * (2.54) * cm / 200.0;
		};

		//CW: 1, CCW: 0
		void Update_Motor_Direction(void) {
			if(Dir == 0) {
				digitalWrite(Dir_Pin, 0);
			};
			if(Dir == 1) {
				digitalWrite(Dir_Pin, 1);
			};
		};

		void Start_Motor_Thread(void) {
			pthread_t thread; //Still need to fix where this is defined
			if (pthread_create(&thread, nullptr, StepperMotor::threadFunctionWrapper, this)) {
				std::cerr << "Error creating ThreadA" << std::endl;
				return ;
			};
			pthread_join(thread, nullptr);
		};
		
		static void* threadFunctionWrapper(void* arg) {
			return static_cast<StepperMotor*>(arg)->Run_Motor_New();
		};
		
		void Motor_Control(int Distance, int Direction) {
			Dir = Direction;
			Update_Motor_Direction();
			Set_Motor_Distance(Distance);
			//~ Start_Motor_Thread();
			Start_Motor_Thread();
		};
		
		//Not sure yet how exactly I should handle running threads outside of a function
		//Maybe the thread variable should be kept inside this class
		

		
};


int main(void) {
	
	if(wiringPiSetup() == -1) {
		exit(1);
	}
	
	StepperMotor MotorA(26, 22, 23, 2500.0, 2, 3);
	MotorA.Init_Motor();
	MotorA.Motor_Control(10, 1);

	
	
	return 0;
};

















