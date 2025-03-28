#include <wiringPi.h>
#include <wiringSerial.h>
#include <iostream>
#include <string>

int main() {
    int serialPort;
    if ((serialPort = serialOpen("/dev/ttyAMA0", 115200)) < 0) {
        std::cerr << "Unable to open serial device" << std::endl;
        return 1;
    }

    if (wiringPiSetup() == -1) {
        std::cerr << "Unable to start wiringPi" << std::endl;
        return 1;
    }

    // Send command to STM32 to start ADC data collection
    serialPuts(serialPort, "START_ADC\n");

    // Wait for data from STM32
    std::string data;
    while (true) {
        if (serialDataAvail(serialPort)) {
            char c = serialGetchar(serialPort);
            if (c == '\n') {
                break;
            }
            data += c;
        }
    }

    std::cout << "Received ADC data: " << data << std::endl;

    serialClose(serialPort);
    return 0;
}