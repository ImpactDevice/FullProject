#include <wiringPi.h>
#include <pthread.h>
#include <bits/stdc++.h>
#include <unistd.h> // For usleep fucntion
//~ #include <pigpio.h>
#include <iostream>
//Making Some small change

#define Stepper_P0 0
#define Stepper_P1 1
#define Stepper_P2 2
#define Stepper_P3 3
#define StepperA_Clock 26  
#define Stepper_Dir_Pin 0
#define Stepper_Control_Pin 1

// 1/200 * Lead this gives you distance/step
//.33 inches


//============Functions===============
void* Run_Stepper_CW(void*);
void* Run_Stepper_CCW(void*);
void Init_Stepper(void);
void Init_StepperA_Clock(void);
void Init_Direction(void);
void Init_Direction(void);

//==========Structs===================
struct StepperData {
	//~ int Pin0 = 0;
	//~ int Pin1 = 1;
	//~ int Pin2 = 2;
	//~ int Pin3 = 3;
	//~ int Clock_Pin = 4;
	
	int Steps = 200;
	int Dir = 1;
	float Clock_hz = 2500.0;
	//~ float Clock_hz = 264.0;
	//~ int High_Time = 3; //us
	//~ int Low_Time = 1;
	
	int Delta_Steps = 0;
};

//============Global Vars============

static pthread_t Stepper_ThreadA;
static StepperData StepperA;

void Init_Stepper(void) {
	pinMode(Stepper_P0, OUTPUT);
	pinMode(Stepper_P1, OUTPUT);
	pinMode(Stepper_P2, OUTPUT);
	pinMode(Stepper_P3, OUTPUT);
};

void Init_Stepper_Direction(void) {
	pinMode(Stepper_Dir_Pin, OUTPUT);
};

void Init_Stepper_Control(void) {
	pinMode(Stepper_Control_Pin, OUTPUT);
	digitalWrite(Stepper_Control_Pin, LOW);
};
	

void Init_StepperA_Clock(void) {
	pinMode(StepperA_Clock, GPIO_CLOCK);
	
	//250khz high: 2us low 2us
	gpioClockSet(StepperA_Clock, StepperA.Clock_hz); //250khz haft of the max	
};

//Closer to 425khz right now 64, 32 , 1
// 140khz 192, 96, 1

//its supposed to be PWM_Frequency = base_Frequency (27Mhz)/(divisor*Range)
//to get Range do (27Mhz)/(PWM_Frequency) set divisor to 1
//Max range is 1024
void Init_Clock_PWM(void) {
	pinMode(StepperA_Clock, PWM_OUTPUT);
	
	float estimate = (27000000.0/(StepperA.Clock_hz*32.0));
	std::cout << "DIVISOTR";
	std::cout << estimate;
	std::cout << "\n";
	int Divisor = round(estimate);
	std::cout << "Real Divisor";
	std::cout << Divisor;
	pwmSetMode(PWM_MODE_MS);
	
	//~ pwmSetRange(192);
	//~ pwmSetClock(1);
	//~ pwmWrite(StepperA_Clock, 96);
	
	//
	//At 1A max frequency is ~8khz tested using a waveform generator
	//Then we lowered amps to see if it affected max frequency
	//At 0.5 Amps we got 9khz
	
	//this is currently set for 2.5khz
	//Range
	pwmSetRange(108);
	//clock divisor
	pwmSetClock(100); 
	//Duty cycle part set to 1/2 of range
	pwmWrite(StepperA_Clock, 54);
	
	//~ pwmSetRange(1024);
	//~ pwmSetClock(100);
	//~ pwmWrite(StepperA_Clock, 512);
	//27MHz is base
}

void Init_Motor(void) {
	//~ Init_StepperA_Clock();
	Init_Clock_PWM();
	Init_Stepper_Direction();
	Init_Stepper_Control();
};

void Run_Motor_Forever(void) {
	digitalWrite(Stepper_Dir_Pin, HIGH);
	digitalWrite(Stepper_Control_Pin, LOW);
};

void* Run_Motor_New(void* arg) {
	StepperData* data = static_cast<StepperData*>(arg);
	float Period_Sec = 1.0/(data->Clock_hz);
	int Period_us = (data->Steps)*round(Period_Sec*1000000);
	std::cout << (data->Clock_hz);
	std::cout << "\n";
	std::cout << Period_Sec;
	std::cout << "\n";
	std::cout << Period_us;
	digitalWrite(Stepper_Control_Pin, HIGH);
	//~ usleep(Period_us);
	delayMicroseconds(Period_us);
	digitalWrite(Stepper_Control_Pin, LOW);
	std::cout << "\n";
	std::cout << "Done With Motor Thread";
	return nullptr;
}; 

void Turn_Motor(void) {
	if(StepperA.Dir == 0) {
		digitalWrite(Stepper_Dir_Pin, 0);
	};
	if(StepperA.Dir == 1) {
		digitalWrite(Stepper_Dir_Pin, 1);
	};
	if (pthread_create(&Stepper_ThreadA, nullptr, Run_Motor_New, &StepperA)) {
		std::cerr << "Error creating ThreadA" << std::endl;
		return ;
	};
};
	
	


//~ void* Run_Stepper_CW(void* arg) {
	//~ StepperData* data = static_cast<StepperData*>(arg);
	//~ for(int i=0; i< data->Steps; i++) {
		//~ //Increament Counter
		//~ data->cStep = (data->cStep + 1) % 4;
		
		//~ //Set all To Low
		//~ digitalWrite(Stepper_P0, LOW);
		//~ digitalWrite(Stepper_P1, LOW);
		//~ digitalWrite(Stepper_P2, LOW);
		//~ digitalWrite(Stepper_P3, LOW);
		
		//~ usleep(data->Low_Time); //1ms might need this
		
		//~ //Switch which pin is High
		//~ digitalWrite(Stepper_P0+data->cStep, HIGH);
		
		//~ //usleep(1000000); //1 sec delay to determine the speed of the motor
		//~ usleep(data->High_Time); //1ms
	//~ }
	//~ // Might just put this into the loop ++data->Delta_Steps;
	//~ data->Delta_Steps = data->Delta_Steps + data->Steps;
	//~ return nullptr;
//~ };

//~ void* Run_Stepper_CCW(voi argd*) {
	//~ StepperData* data = static_cast<StepperData*>(arg);
	//~ for(int i=0; i< data->Steps; i++) {
		//~ //Decrease Counter
		//~ data->cStep = (data->cStep + 3) % 4;
		
		//~ //Set all To Low
		//~ digitalWrite(Stepper_P0, LOW); //Edit this later to make it valid for multiple motors
		//~ digitalWrite(Stepper_P1, LOW);
		//~ digitalWrite(Stepper_P2, LOW);
		//~ digitalWrite(Stepper_P3, LOW);
		
		//~ //Wait for the Rest of the Period
		//~ usleep(data->Low_Time); //1ms might need this
		
		//~ //Switch which pin is High
		//~ digitalWrite(Stepper_P0+data->cStep, HIGH);
		
		//~ //Wait untill Clock is Low
		//~ usleep(data->High_Time);
	//~ }
	//~ data->Delta_Steps = data->Delta_Steps - data->Steps;
	//~ return nullptr;
//~ };
	
//~ void Create_StepperA_Thread(void) {
	//~ if(*Dir == 0) {
		//~ pthread_create(&Stepper_ThreadA, nullptr, Run_Stepper_CW, &StepperA);
	//~ };
	//~ //if(*Dir == 1) {
	//~ //	pthread_create(&Stepper_ThreadA, nullptr, Run_Stepper_CCW, &StepperA);
	//~ //};
	//~ if (pthread_create(&Stepper_ThreadA, nullptr, Run_Stepper_CW, &StepperA)) {
		//~ std::cerr << "Error creating ThreadA" << std::endl;
		//~ return ;
	//~ };
	
//~ };





int main(void) {
	if(wiringPiSetup() == -1) {
		exit(1);
	}
	

	Init_Motor();
	//~ Run_Motor_Forever();
	//~ Turn_Motor();
	
	if(StepperA.Dir == 0) {
		digitalWrite(Stepper_Dir_Pin, 0);
	};
	if(StepperA.Dir == 1) {
		digitalWrite(Stepper_Dir_Pin, 1);
	};
	if (pthread_create(&Stepper_ThreadA, nullptr, Run_Motor_New, &StepperA)) {
		std::cerr << "Error creating ThreadA" << std::endl;
		return 1;
	};
	//~ Run_Motor_Forever();
	//~ Create_StepperA_Thread();

	//wait for the thread to end
	//~ if(pthread_join(Stepper_ThreadA, nullptr)) {
		//~ std::cerr << "Error Joining thread" << std::endl;
	//~ };
	pthread_join(Stepper_ThreadA, nullptr);
	int delta = StepperA.Delta_Steps;
	std::cout << "the delta";
	std::cout << "\n";
	std::cout << delta;
	
	pinMode(StepperA_Clock, INPUT);
	
	return 0;
};
