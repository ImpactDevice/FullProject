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

//Distance Sensor: Ethernet
//Camera: USB
//

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





//~ int SPI_Channel = 0;
//~ int Channel = 0;
//~ int Speed = 1000000;
//~ float V_ref = 3.3;
//~ std::vector<float> data;

		//~ float readADC(int channel) {
			//~ uint8_t buffer[3];
			//~ buffer[0] = 1; //starting bit
			//~ buffer[1] = (0x08 | channel) << 4; // Channel Number
			//~ buffer[2] = 0; // Don't care byte
			
			//~ for (int i = 0; i< 3; i++) {
				//~ std::cout << "0x";
				//~ std::cout << std::hex;
				//~ std::cout << (int)buffer[i];
				//~ std::cout << " ";
				
			//~ };
			//~ std::cout << "\n";
			
			//~ wiringPiSPIDataRW(SPI_CHANNEL, buffer, 3);
			
			//~ int result = ((buffer[1] & 0x03) << 8) | buffer[2];
			//~ static float Voltage = (result/V_ref) * 1023.0;
			//~ float Voltage = (result/1023.0)*V_ref;
			//~ std::cout << "\nADC VALUE\n";
			//~ std::cout << result;
			//~ std::cout << "\n";
			//~ std::cout << "\nVoltage Value\n";
			//~ std::cout << Voltage;
			//~ std::cout << "\n";
			//~ return Voltage;was not declared in this scope; did you mean ‘nextup’?

		//~ };
		
		//~ void I_Running()’:
//~ StepperMotorClassNew.cpp:63:9: warning: cnit_Array(void) {
			//~ std::vector<float> data2;
		//~ };
		
		//~ void Add_Data_To_Array(float Num) {
			//~ data.push_back(Num);
		//~ };
		
		//~ v		//~ for (int i = 0; i< 3; i++) {
				//~ std::cout << "0x";
				//~ std::cout << std::hex;
				//~ std::cout << (int)buffer[i];
				//~ std::cout << " ";
				
			//~ };
			//~ std::cout << "\n";oid Create_File(void) {
			//~ std::filesystem::create_directory("Data_Folder");
		//~ };
		
		//~ void Save_Data_To_File(void) {
			//~ std::ofstream file("Data_Folder/DataForTest1.csv");
			
			
			//~ for(size_t i = 0; i < data.size(); ++i) {
				//~ file << data[i];
				//~ if (i < data.size() - 1) {
					//~ file << ",";
				//~ }
			//~ }
			//~ file << "\n";
		
			
			//~ file.close();
		//~ };
		
		
		//~ void* Collect_Data() {
			//~ int i = 0;
			//~ float Volts;
			//~ while(i < 20000) {
				//~ i++;
				//~ Volts = readADC(0);
				//~ Add_Data_To_Array(Volts);
			//~ };
			//~ Save_Data_To_File();

			//~ return nullptr;
		//~ };
		
		//~ void Start_ADC_Thread(void) {
			//~ pthread_t thread; //Still need to fix where this is defined
			//~ if (pthread_create(&thread, nullptr, SPIthreadFunctionWrapper, this)) {
				//~ std::cerr << "Error creating ThreadA" << std::endl;
				//~ return ;
			//~ };
			//~ pthread_join(thread, nullptr);
		//~ };
		
		//~ static void* threadFunctionWrapper(void* arg) {
			//~ return static_cast<SPIComms*>(arg)->Collect_Data();
		//~ };
		

		//~ void Setup(void) {
			//~ if(wiringPiSetup() == -1) {
				//~ std::cerr << "Failed to Initialize WiringPi\n";
				//~ exit(1);
			//~ };
			//~ if (wiringPiSPISetup(SPI_CHANNEL, Speed) == -1) {
				//~ std::cerr << "Failed to Setup SPI" << std::endl;
			//~ };
			//~ Init_Array();

		//~ };
		
void TempCameraRun(void) {
	system("/home/ethankotrba/Desktop/example-v3-linux/out/bin/Trigger.sh");	
}
		
		
void TempCameraSetup(void) {
    FILE *fp;
    char result[128];

	const char *script_dir = "/home/ethankotrba/Desktop/example-v3-linux/out/bin";
    if (chdir(script_dir) != 0) {
        perror("chdir() to script directory failed");
        return 1;
	}
	    // Run the bash script and capture its output
    fp = popen("/home/ethankotrba/Desktop/example-v3-linux/out/bin/Connect.sh", "r");
    if (fp == NULL) {
        perror("popen() failed");
        return 1;
    }

    // Read the output from the script
    if (fgets(result, sizeof(result), fp) != NULL) {
		        if (strcmp(result, "Error: cannot connect to the camera\n") == 0) {
					printf("The script returned an error: %s", result);
					// Additional error handling code here
					pclose(fp);
					return 1;
				}
            
    pclose(fp);
    }
};


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
//int Motor_Clock_Pin = 26;
float Motor_Clock_hz = 2500.0;
//int Motor_Dir_Pin = 22;
//int Motor_Control_Pin = 23;
//int Motor_MS1_Pin = 2;
//int Motor_MS2_Pin = 3;

//StepperMotor MotorA(26, 22, 23, 2500.0, 2, 3);
//StepperMotor MotorA(Motor_Clock_Pin, Motor_Dir_Pin, Motor_Control_Pin, Motor_Clock_hz, Motor_MS1_Pin, Motor_MS2_Pin);
StepperMotor MotorA(MOTOR_CLOCK_PIN, MOTOR_DIR_PIN, MOTOR_CONTROL_PIN, Motor_Clock_hz, MOTOR_MS1_PIN, MOTOR_MS2_PIN);
int Is_Done = 0;
volatile int lastInterruptTime0 = 0;
volatile int lastInterruptTime1 = 0;


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
	MotorA.Update_Motor_Direction(1); //1 is Down
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
			
			//ADD: Load Cell Start Data Collection
			//ADD: Start Recording Laser Distance Sensor
			//ADD: Start Recording on Camera
			if(Enable_Camera == 'on') {
				TempCameraRun();
			}
			Turn_Off_EM();
			//ADD: 2 different Methods for choosing when to end
			//ADD: Least Square root Filter
			//ADD: Kalman-Filter On Data
			//ADD: Orginize Data For CSV
			//ADD: Write CSV to memory
			
			
			Count++;
		};
		lastInterruptTime0 = Interrupt_Time0;
	};
	

};









void Stop_Motor_Shell(void) {
	MotorA.Stop_Motor();
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
	if( wiringPiISR(MID_LS_PIN, INT_EDGE_RISING, Raise_Arm) < 0) {
		std::cerr << "Failed to set up ISR On PIN: " << MID_LS_PIN << std::endl;
	};
};


void Init_All(void) {
	Init_EM();
	Init_MotorA();
	Interrupt_Init();
	Top_Interrupt_Init();
	Bot_Interrupt_Init();
	if(Enable_Camera == 'on') {
		TempCameraSetup();
	}
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
	std::cout << "Inside Energy To CM" << std::endl;
	std::cout << "Impact Energy: " << Impact_Energy << std::endl;
	std::cout << "Mass: " << Mass std::endl;
	float cm = Impact_Energy*100/((9.81)*Mass);
	return cm;
};


int main(int argc, char* argv[]) {


	auto args = parseArguements(argc, argv);
	 
	std::string iImpact_Energy = args.count("-Impact-Energy") ? args["-Impact-Energy"] : "0";
	//Might want to redo how -Mass Works down the road to make it more stream lined
	std::string iMass = args.count("-Mass") ? args["-Mass"] : "3.092";
	std::string iMotor_Correction_Factor = args.count("-Motor-Coeff") ? args["-Motor-Coeff"] : "1";
	std::string Enable_Camera = args.count("-Enable-Camera") ? args["-Enable-Camera"] : "1";
	//std::string Camera_Delay = args.count("-Camera-Delay") ? args["-Camera-Delay"] : "0";
	
	Impact_Energy = stof(iImpact_Energy);
	Mass = stof(iMass);
	Motor_Correction_Factor = stof(iMotor_Correction_Factor);
	std::cout << "Impact Energy: " << Impact_Energy << std::endl;
	
	Delta_Height_CM = Energy_To_Cm();

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

		

		







int mainOLD() {
	if(wiringPiSetup() == -1) {
		exit(1);
	};
	//~ if(gpioInitialise() < 0) {
		//~ exit(2);
	//~ };
	
	Init_All();
	Start_Process();



	//~ int argc;
	//~ std::cout << "Press ENTER: ";
	//~ std::cin >> argc;
	while(1) {
		delay(1000);
		if (Is_Done) {
			return 1;
		};
	};
	
	
	

	return 1;
};
