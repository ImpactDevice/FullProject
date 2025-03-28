#include "StepperMotor.h"
#include "../../LimitSwitch/LimitSwitch.h"

#include <wiringPi.h>
#include <pthread.h>
#include <bits/stdc++.h>
#include <unistd.h> // For usleep fucntion
#include <pigpio.h>
#include <iostream>
#include <functional>

#include <fstream>
#include <filesystem>
#include <string>
#include <wiringPiSPI.h>

#include <time.h>
#include <unordered_map>



#define MID_LS_PIN 0
#define TOP_LS_PIN 0
#define BOT_LS_PIN 0

#define SPI_CHANNEL 0
#define SPI_SPEED 1000000

#define MOTOR_CLOCK_PIN 26
#define MOTOR_DIR_PIN 22
#define MOTOR_CONTROL_PIN 23
#define MOTOR_MS1_PIN 2
#define MOTOR_MS2_PIN 3



		
		


//Global Vars:

//==============INPUT VARS=============
float Impact_Energy;
float Mass;
float Motor_Correction_Factor;


float Delta_Height_CM;

int Count = 0;
int Is_Mid_Interrupt = 0;


//===============Saved Vars=================



//====================Motor Vars============
StepperMotor MotorA(MOTOR_CLOCK_PIN, MOTOR_DIR_PIN, MOTOR_CONTROL_PIN, Motor_Clock_hz, MOTOR_MS1_PIN, MOTOR_MS2_PIN);
int Is_Done = 0;
volatile int lastInterruptTime0 = 0;
volatile int lastInterruptTime1 = 0;

void Init_MotorA(void) {
	MotorA.Set_Corr_Factor(Motor_Correction_Factor);
	MotorA.Init_Motor();
	MotorA.Update_Motor_Direction(1);
};

void Start_Process(void) {
	MotorA.Run_Motor_Forever(); //Lower Arm until limit Switch hits
};

void Emergancey_Stop(void) {
	std::cout << "Motor Hit Overdriving Limit Switch" << std::endl;
	MotorA.Stop_Motor(); //this might not work if the thread isn't running
	exit(2);
};	

void Top_Interrupt_Init(void) {
	pinMode(TOP_LS_PIN, INPUT);
	pullUpDnControl(TOP_LS_PIN, PUD_DOWN);
	if( wiringPiISR(TOP_LS_PIN, INT_EDGE_RISING, Emergancey_Stop) < 0) {
		std::cerr << "Failed to set up ISR On PIN: " << TOP_LS_PIN << std::endl;
	};
};

void Bot_Interrupt_Init(void) {
	pinMode(BOT_LS_PIN, INPUT);
	pullUpDnControl(BOT_LS_PIN, PUD_DOWN);
	if( wiringPiISR(BOT_LS_PIN, INT_EDGE_RISING, Emergancey_Stop) < 0) {
		std::cerr << "Failed to set up ISR On PIN: " << BOT_LS_PIN << std::endl;
	};
};

void Interrupt_Init(void) {
	pinMode(MID_LS_PIN, INPUT);
	pullUpDnControl(MID_LS_PIN, PUD_DOWN);
	if( wiringPiISR(MID_LS_PIN, INT_EDGE_RISING, Emergancey_Stop) < 0) {
		std::cerr << "Failed to set up ISR On PIN: " << MID_LS_PIN << std::endl;
	};
};




void Init_All(void) {
	Init_MotorA();
	Interrupt_Init();
	Top_Interrupt_Init();
	Bot_Interrupt_Init();
};

//need a string_to_float()
int string_To_Int(std::string str) {
	try {
		int num = std::stoi(str);
		return num;
	} catch ( const std::invalid_argument& e) {
		std::cerr << "Invalid String to Int: " << e.what() << "\n";
		return -1;
	} catch (const std::out_of_range& e) {
		std::cerr << "Invalid String to Int: " << e.what() << "\n";
		return -1;
	}
};



std::unordered_map<std::string, std::string> parseArguements(int argc, char* argv[]) {
	std::unordered_map<std::string, std::string> args;
	for (int i=1; i < argc; i+=2) {
		if (i + 1 < argc) {
			args[argv[i]] = argv[i+1];
		}
	}
	return args;
	
};


float Convert_To_CM(void):
	if(Units == "inch") {
		return Distance*2.54;
	}
	if(Units == "mm") {
		return Distance/10;
	}
	if(Units == "cm") {
		return Distance;
	} else {
		std::cerr << "Invalid Unit: " << Units << std::endl;
		exit(2);
	}
};
	




int main(int argc, char* argv[]) {


	auto args = parseArguements(argc, argv);
	
	std::string iEngage = args.count("-Engage") ? args["-Engage"] : "1";
	std::string iDir = args.count("-dir") ? args["-dir"] : "up";
	std::string Units = args.count("-Units") ? args["-Units"] : "cm";
	std::string iDistance = args.count("-Distance") ? args["-Distance"] : "0";
	std::string iMotor_Correction_Factor = args.count("-Motor-Coeff") ? args["-Motor-Coeff"] : "1";

	Engage = stoi(iEngage);
	
	if(Engage == 1) {
		MotorA.Disengage_Motor();
	} else {
		MotorA.Engage_Motor();
	}
	if(iDir == 'up') {
		int Dir = 1;
	} else {
		int Dir =0;
	}
	
	Distance = stof(iDistance);	
	Motor_Correction_Factor = stof(iMotor_Correction_Factor);
	Delta_Height_CM = Convert_To_CM();

	std::cout << "DeltaHeight: " << Delta_Height_CM << std::endl;
	 
	 
	if(wiringPiSetup() == -1) {
		exit(1);
	};
	
	
	
	Init_All();
	//Add Something to double check that one of the limitswitches aren't already being triggered
	MotorA.Motor_Control(Delta_Height_CM, Dir);

	return 1;
}
