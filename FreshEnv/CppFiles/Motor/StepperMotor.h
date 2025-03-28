#ifndef STEPPERMOTOR_H
#define STEPPERMOTOR_H
#include <pthread.h>

class StepperMotor {

	private:
		int Clock_Pin;
		int Dir_Pin;
		int Control_Pin;
		int MS1_Pin;
		int MS2_Pin;
		float Clock_hz = 2500.0;
		int Dir = 0;
		int Step_Size = 8.0;
		int Steps_Per_Rev = 120; // in 1/2 stepping???
		float Corr_Fact = 1; //Redu this
		int Run_Time_us;
		float Screw_Lead_in = 0.333;
		pthread_t Thread;
				//thread var here?
		int Steps;

	public:
		
		StepperMotor(int ClockPin, int DirPin, int ControlPin, float ClockFrequency, int MS1, int MS2);
		void Init_Direction(void);
		void Init_Control(void);
		void Init_Clock_PWM(void);
		void Init_Motor(void);
		void Set_Corr_Factor(float Factor);
		void Stop_Motor(void);
		void Disengage_Motor(void);
		void Engage_Motor(void);
		void Run_Motor_Forever(void);
		void* Run_Motor_New();
		void Set_Motor_Distance(float cm);
		void Update_Motor_Direction(int dir);
		//static void* threadFunctionWrapper(void* arg);
		static void* threadFunctionWrapper(void* arg);
		void Motor_Control(int Distance, int Direction);
		void Start_Motor_Thread(void);
		int Is_Thread_Running(void);
		

		


		

		
};



#endif
