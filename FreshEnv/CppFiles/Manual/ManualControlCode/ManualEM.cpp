#include <wiringPi.h>
#include <bits/stdc++.h>
#include <unistd.h> // For usleep fucntion
#include <iostream>
#include <functional>

#include <fstream>
#include <filesystem>
#include <string>
#include <unordered_map>



#define EM_Pin 24



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


std::unordered_map<std::string, std::string> parseArguements(int argc, char* argv[]) {
	std::unordered_map<std::string, std::string> args;
	for (int i=1; i < argc; i+=2) {
		if (i + 1 < argc) {
			args[argv[i]] = argv[i+1];
		}
	}
	return args;
	
};


int main(int argc, char* argv[]) {


	auto args = parseArguements(argc, argv);
	 
	std::string Enable = args.count("-Enable") ? args["-Enable"] : "0";
	
	if(wiringPiSetup() == -1) {
		exit(1);
	};
	
	Init_EM();
	if(Enable == "0") {
		Turn_Off_EM();
	} else {
		Turn_On_EM();
	};
	
	return 1;
}


