#include <wiringPi.h>
#include <wiringPiSPI.h>
#include <iostream>

#include <chrono>

#include <fstream>
#include <filesystem>

#include <pthread.h>
#include <bits/stdc++.h>

//Stuff For Changing Data

#define SPI_CHANNEL 0
#define SPI_SPEED 3600000
std::vector<float> RawData;
std::vector<float> FilterData1;
std::vector<float> FilterData2;

class MovingAverageFilter {
	public:
		MovingAverageFilter(int window_size) : window_size(window_size) {}
		
		double apply(double new_value) {
			values.push_back(new_value);
			if(values.size() > window_size) {
				values.pop_front();
			};
			
			// calculate the average
			double sum = 0;
			for (double val: values) {
				sum += val;
			}
			return sum / values.size();
			
		};

	private:
		int window_size;
		std::deque<double> values;
};

class ExpMovingAve{
	public:
		std::vector<float> rawData;
		ExpMovingAve(double alpha) : alpha(alpha), last_filtered_value() {}
		double apply(double new_value) {
			last_filtered_value = alpha * new_value + (1 - alpha) * last_filtered_value;
			return last_filtered_value;
		}
	private:
		double alpha;
		double last_filtered_value;
};

//SPI1 = gpio 7 - 11
class SPIComms {
	
	public:
		int SPI_Channel = 0;
		int Channel = 0;
		int Speed = 1000000;
		float V_ref = 3.3;
		std::vector<float> data;
		//~ std::vector<std::float> data;
		
		float readADC(int channel) {
			auto start = std::chrono::high_resolution_clock::now();
			uint8_t buffer[3];
			buffer[0] = 1; //starting bit
			buffer[1] = (0x08 | channel) << 4; // Channel Number
			buffer[2] = 0; // Don't care byte
			
			//~ for (int i = 0; i< 3; i++) {
				//~ std::cout << "0x";
				//~ std::cout << std::hex;
				//~ std::cout << (int)buffer[i];
				//~ std::cout << " ";
				
			//~ };
			//~ std::cout << "\n";
			
			wiringPiSPIDataRW(SPI_CHANNEL, buffer, 3);
			
			int result = ((buffer[1] & 0x03) << 8) | buffer[2];
			//~ static float Voltage = (result/V_ref) * 1023.0;
			float Voltage = (result/1023.0)*V_ref;
			//~ std::cout << "\nADC VALUE\n";
			//~ std::cout << result;
			//~ std::cout << "\n";
			//~ std::cout << "\nVoltage Value\n";
			//~ std::cout << Voltage;
			//~ std::cout << "\n";
			auto end = std::chrono::high_resolution_clock::now();
			std::chrono::duration<double> duration = end - start;
			std::cout << "\n;";
			std::cout << duration.count();
			std::cout << "\n";
			return Voltage;
		};
		
		void Init_Array(void) {
			std::vector<float> data2;
		};
		
		void Add_Data_To_Array(float Num) {
			data.push_back(Num);
		};
		
		void Create_File(void) {
			std::filesystem::create_directory("Data_Folder");
		};
		
		void Save_Data_To_File(void) {
			std::ofstream file("Data_Folder/Data3_10kneg1vref.csv");
			
			
			for(size_t i = 0; i < data.size(); ++i) {
				file << data[i];
				if (i < data.size() - 1) {
					file << "\n";
				}
			}
			//~ file << "\n";
		
			
			file.close();
		};
		
		void Save_All_Data_To_File(void) {
			std::ofstream file("Data_Folder/Data3_10kneg1vref.csv");
			
			
			for(size_t i = 0; i < data.size(); ++i) {
				file << data[i];
				if (i < data.size() - 1) {
					file << "\n";
				}
			}
			//~ file << "\n";
		
			
			file.close();
		};
		
		
		void* Collect_Data() {
			int i = 0;
			float Volts;
			while(i < 100000) {
				i++;
				Volts = readADC(0);
				Add_Data_To_Array(Volts);
			};
			//Save_Data_To_File();

			return nullptr;
		};
		
		void Start_ADC_Thread(void) {
			pthread_t thread; //Still need to fix where this is defined
			if (pthread_create(&thread, nullptr, SPIComms::threadFunctionWrapper, this)) {
				std::cerr << "Error creating ThreadA" << std::endl;
				return ;
			};
			pthread_join(thread, nullptr);
			std::cout << "Data Collection Is Done\n";
		};
		
		static void* threadFunctionWrapper(void* arg) {
			return static_cast<SPIComms*>(arg)->Collect_Data();
		};
		

		void Setup(void) {
			if(wiringPiSetup() == -1) {
				std::cerr << "Failed to Initialize WiringPi\n";
				exit(1);
			};
			if (wiringPiSPISetup(SPI_CHANNEL, SPI_SPEED) == -1) {
				std::cerr << "Failed to Setup SPI" << std::endl;
			};
			Init_Array();

		};
};

void Save_All_Data_To_File(void) {
			std::ofstream file("Data_Folder/BelowBlock.csv");
			
			
			for(size_t i = 0; i < RawData.size(); ++i) {
				file << RawData[i];
				//~ file << ",";
				//~ file << FilterData1[i];
				//~ file << ",";
				//~ file << FilterData2[i];


				if (i < RawData.size() - 1) {
					file << "\n";
				}
			}
			//~ file << "\n";
		
			
			file.close();
};


int main() {
	MovingAverageFilter Filter1(5);
	ExpMovingAve Filter2(0.1);
	SPIComms ADC1;
	ADC1.Setup();
	ADC1.Start_ADC_Thread();
	
	std::cout << "Performing Filters";
	RawData = ADC1.data;
	for (double value : RawData) {
		double filter1_value = Filter1.apply(value);
		double filter2_value = Filter2.apply(value);
		FilterData1.push_back(filter1_value);
		FilterData2.push_back(filter2_value);
	};
	Save_All_Data_To_File();
	
	//~ for( int i=0; i<7; i++) {
		//~ ADC1.readADC(i);
	//~ };
	
	 
	
};











