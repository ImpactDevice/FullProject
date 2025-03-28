#include "RaspberryPiIOLink.h"
#include <wiringPi.h>
#include <wiringSerial.h>
#include <iostream>

RaspberryPiIOLink::RaspberryPiIOLink(const std::string& device, int baudRate)
    : device(device), baudRate(baudRate), running(false) {}

RaspberryPiIOLink::~RaspberryPiIOLink() {
    stop();
}

void RaspberryPiIOLink::start() {
    running = true;
    workerThread = std::thread(&RaspberryPiIOLink::run, this);
}

void RaspberryPiIOLink::stop() {
    running = false;
    if (workerThread.joinable()) {
        workerThread.join();
    }
}

std::string RaspberryPiIOLink::getData() const {
    return data;
}

void RaspberryPiIOLink::run() {
    int serialPort;
    if ((serialPort = serialOpen(device.c_str(), baudRate)) < 0) {
        std::cerr << "Unable to open serial device" << std::endl;
        return;
    }

    if (wiringPiSetup() == -1) {
        std::cerr << "Unable to start wiringPi" << std::endl;
        return;
    }

    // Send command to start data collection
    serialPuts(serialPort, "START\n");

    // Wait for data from the sensor
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