#include <wiringPi.h>
#include <wiringPiSPI.h>
#include "StepperMotor.h"


#define EM_Pin 24

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


int EM_Enable;
int Motor;
float MotorDistance;


float Motor_Clock_hz = 2500.0;
StepperMotor MotorA(MOTOR_CLOCK_PIN, MOTOR_DIR_PIN, MOTOR_CONTROL_PIN, Motor_Clock_hz, MOTOR_MS1_PIN, MOTOR_MS2_PIN);


void Init_EM(void) {
	pinMode(EM_Pin, OUTPUT);
	digitalWrite(EM_Pin, LOW);
};

void Turn_On_EM(void) {
	digitalWrite(EM_Pin, HIGH);
};

void Turn_Off_EM(void) {
	digitalWrite(EM_Pin, LOW);
};

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
	

void Raise_Arm(void) {
	int Interrupt_Time0 = millis();
	if(!Is_Mid_Interrupt) {
		Is_Mid_Interrupt = 1;
	
		if (Interrupt_Time0 - lastInterruptTime0 > 2000) { //Avoid Bounceing Switch
				//pthread_cancel(MotorA.Thread); have to fix the Thread declaration in class
			std::cout << "Count: " << Count << std::endl;
			std::cout << "Just before Stop Motor func";
			MotorA.Stop_Motor(); //this might not work if the thread isn't running
			//Might Add a thing so that after the switch is triggered it moves slightly closer for accuracey purposes.
			Turn_On_EM();
			//------------------------ADD It to make sure it grabs the distance measurement from the Sensor Here---------------------------------
			MotorA.Motor_Control(Delta_Height_CM, 0);
			
			//============inserted a delay for testing purposes==============
			std::cout << "quick pause at top: 1 sec" << std::endl;
			delay(1000);
			
			//---------------------------------Turn On all the sensors First here or have Turn_Off_EM outside of this Inturrupt
			Turn_Off_EM();
			Count++;
		};
		lastInterruptTime0 = Interrupt_Time0;
	};
	

};

void Raise_Arm_gpio(int gpio, int level, uint32_t tick) {
	
	int Interrupt_Time0 = millis();
	if (Interrupt_Time0 - lastInterruptTime0 > 200) {
			//pthread_cancel(MotorA.Thread); have to fix the Thread declaration in class
		std::cout << "Just before Stop Motor func OLD";
		MotorA.Stop_Motor(); //this might not work if the thread isn't running
		Turn_On_EM();
		MotorA.Motor_Control(1, 1);
		Turn_Off_EM();
	};
	lastInterruptTime0 = Interrupt_Time0;
	

};


void Deregister_Interrupt(int pin) {
	char command[50];
	//~ snprintf(command, sizeof(command), "/usr/local/bin/gpio edge %d none", pin);
	//~ system(command);

};

void End_Program(void) {
	std::cout << "ENDING PROGRAM";
	int Interrupt_Time1 = millis();
	if (Interrupt_Time1 - lastInterruptTime1 > 200) {
		MotorA.Stop_Motor();
		Deregister_Interrupt(MID_LS_PIN);
		Turn_Off_EM();
		Deregister_Interrupt(1);
		//gpioTerminate();
		Is_Done = 1;
	};
	lastInterruptTime1 = Interrupt_Time1;

}

//~ void End_Program_gpio(int gpio, int level, uint32_t tick) {
	//~ std::cout << "ENDING PROGRAM";
	//~ int Interrupt_Time1 = millis();
	//~ if (Interrupt_Time1 - lastInterruptTime1 > 200) {
		//~ MotorA.Stop_Motor();
		//~ Deregister_Interrupt(LS_Pin1);
		//~ Turn_Off_EM();
		//~ Deregister_Interrupt(1);
		//~ gpioTerminate();
		//~ Is_Done = 1;
	//~ };
	//~ lastInterruptTime1 = Interrupt_Time1;
//~ }

//~ void Interrupt_Handler(void) {

void Stop_Motor_Shell(void) {
	MotorA.Stop_Motor();
};
	

void Top_Bot_Interrupt_Init(void) {
	pinMode(MID_LS_PIN, INPUT);
	pullUpDnControl(MID_LS_PIN, PUD_DOWN);
	if( wiringPiISR(MID_LS_PIN, INT_EDGE_RISING, Stop_Motor_Shell) < 0) {
		std::cerr << "Failed to set up ISR On PIN: " << MID_LS_PIN << std::endl;
	};
};

void Interrupt_Init(void) {
	pinMode(MID_LS_PIN, INPUT);
	pullUpDnControl(MID_LS_PIN, PUD_DOWN);
	if( wiringPiISR(MID_LS_PIN, INT_EDGE_RISING, Raise_Arm) < 0) {
		std::cerr << "Failed to set up ISR On PIN: " << MID_LS_PIN << std::endl;
	};
	//~ pinMode(1, INPUT);
	//~ pullUpDnControl(1, PUD_DOWN);
	//~ if( wiringPiISR(1, INT_EDGE_RISING, End_Program) < 0) {
		//~ std::cerr << "Failed to set up ISR On PIN: " << 1 << std::endl;
	//~ };
};

//~ void Interrupt_Init_pigpio(void) {
	//~ gpioSetMode(LS_Pin1, PI_INPUT);
	//~ pullUpDnControl(LS_Pin1, PUD_DOWN);
	//~ if( gpioSetISRFunc(LS_Pin1, RISING_EDGE, 0, Raise_Arm_gpio) != 0) {
		//~ std::cerr << "Failed to set up ISR On PIN: " << LS_Pin1 << std::endl;
	//~ };
	//~ gpioSetMode(1, PI_INPUT);
	//~ pullUpDnControl(1, PUD_DOWN);
	//~ if( gpioSetISRFunc(1, RISING_EDGE, 0, End_Program_gpio) != 0) {
		//~ std::cerr << "Failed to set up ISR On PIN: " << 1 << std::endl;
	//~ };
//~ };


void Init_All(void) {
	Init_EM();
	Init_MotorA();
	Interrupt_Init();
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

float Energy_To_Cm(void) {
	float cm = Impact_Energy/((9.81)*Mass*100);
	return cm;
};


int main(int argc, char* argv[]) {


	auto args = parseArguements(argc, argv);
	 
	std::string iImpact_Energy = args.count("-Motor") ? args["-Motor"] : "-1";
	std::string iMass = args.count("-EM") ? args["-EM"] : "0";
	std::string iMotor_Correction_Factor = args.count("-Motor-Coeff") ? args["-Motor-Coeff"] : "1";
	std::string iMotor_Correction_Factor = args.count("-Motor-Dis") ? args["-Motor-Dis"] : "0";
	//std::string Enable_Camera = args.count("-Enable-Camera") ? args["-Enable-Camera"] : "off";
	//std::string Camera_Delay = args.count("-Camera-Delay") ? args["-Camera-Delay"] : "0";
	
	Impact_Energy = stof(iImpact_Energy);
	Mass = stof(iMass);
	Motor_Correction_Factor = stof(iMotor_Correction_Factor);
	std::cout << "Impact Energy: " << Impact_Energy << std::endl;
	
	Delta_Height_CM = Energy_To_Cm();
	Delta_Height_CM = 6.58;
	std::cout << "DeltaHeight: " << Delta_Height_CM << std::endl;
	 
	 
	if(wiringPiSetup() == -1) {
		exit(1);
	};
	
	Init_All();
	Start_Process();
	
	//Wait for the Whole Test to Finish
	//Later this can be its own function and be able to return Error Nums
	while(1) {
		delay(1000);
		if (Is_Done) {
			//I need to Lose the wiringPiSetup() stuff first
			return 1;
		};
	};
	
	
	
	//I need to Lose the wiringPiSetup() stuff first
	return 1;
}
