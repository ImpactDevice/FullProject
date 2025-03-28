#include "RaspberryPiSerial.h"
#include <wiringPi.h>
#include <wiringSerial.h>
#include <iostream>

RaspberryPiSerial::RaspberryPiSerial(const std::string& device, int baudRate)
    : device(device), baudRate(baudRate), running(false) {}

RaspberryPiSerial::~RaspberryPiSerial() {
    stop();
}

void RaspberryPiSerial::start() {
    running = true;
    workerThread = std::thread(&RaspberryPiSerial::run, this);
}

void RaspberryPiSerial::stop() {
    running = false;
    if (workerThread.joinable()) {
        workerThread.join();
    }
}

std::string RaspberryPiSerial::getData() const {
    return data;
}

void RaspberryPiSerial::run() {
    int serialPort;
    if ((serialPort = serialOpen(device.c_str(), baudRate)) < 0) {
        std::cerr << "Unable to open serial device" << std::endl;
        return;
    }

    if (wiringPiSetup() == -1) {
        std::cerr << "Unable to start wiringPi" << std::endl;
        return;
    }

    // Send command to STM32 to start ADC data collection
    serialPuts(serialPort, "START_ADC\n");

    // Wait for data from STM32
    while (running) {
        if (serialDataAvail(serialPort)) {
            char c = serialGetchar(serialPort);
            if (c == '\n') {
                break;
            }
            data += c;
        }
    }

    serialClose(serialPort);
}