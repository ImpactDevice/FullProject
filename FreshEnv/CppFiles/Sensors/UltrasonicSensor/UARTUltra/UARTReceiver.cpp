#include "UARTReceiver.h"
#include <wiringPi.h>
#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>

UARTReceiver::UARTReceiver(const std::string& device, int baud_rate, int stop_bits, char parity)
    : device_(device), baud_rate_(baud_rate), stop_bits_(stop_bits), parity_(parity), running_(false) {
    // Setup WiringPi
    if (wiringPiSetup() == -1) {
        std::cerr << "WiringPi setup failed!" << std::endl;
        throw std::runtime_error("WiringPi setup failed!");
    }

    // Open serial port
    if ((serial_port_ = open(device_.c_str(), O_RDWR | O_NOCTTY | O_NDELAY)) == -1) {
        std::cerr << "Unable to open serial device!" << std::endl;
        throw std::runtime_error("Unable to open serial device!");
    }

    // Setup serial port
    setupSerialPort();
}

UARTReceiver::~UARTReceiver() {
    stopReceiving();
    close(serial_port_);
}

void UARTReceiver::setupSerialPort() {
    struct termios options;
    tcgetattr(serial_port_, &options);

    // Set baud rate
    cfsetispeed(&options, baud_rate_);
    cfsetospeed(&options, baud_rate_);

    // Set stop bits
    options.c_cflag &= ~CSTOPB;
    if (stop_bits_ == 2) {
        options.c_cflag |= CSTOPB;
    }

    // Set parity
    options.c_cflag &= ~PARENB;
    options.c_cflag &= ~PARODD;
    if (parity_ == 'E') {
        options.c_cflag |= PARENB;
    } else if (parity_ == 'O') {
        options.c_cflag |= PARENB;
        options.c_cflag |= PARODD;
    }

    // Set the new options for the port
    tcsetattr(serial_port_, TCSANOW, &options);
}

void UARTReceiver::startReceiving() {
    running_ = true;
    receive_thread_ = std::thread(&UARTReceiver::receiveData, this);
}

void UARTReceiver::stopReceiving() {
    running_ = false;
    if (receive_thread_.joinable()) {
        receive_thread_.join();
    }
}

//void UARTReceiver::receiveData() {
    //while (running_) {
        //if (serialDataAvail(serial_port_)) {
            //char received_char = serialGetchar(serial_port_);
            //std::cout << "Received: " << received_char << std::endl;

            //// Lock the mutex before modifying the received data vector
            //std::lock_guard<std::mutex> lock(data_mutex_);
            //received_data_.push_back(received_char);
        //}
        //delay(100); // Short delay to prevent high CPU usage
    //}
//}

void UARTReceiver::receiveData() {
    char buffer[256];
    while (running_) {
        int bytes_available = read(serial_port_, buffer, sizeof(buffer));
        if (bytes_available > 0) {
            std::lock_guard<std::mutex> lock(data_mutex_);
            received_data_.insert(received_data_.end(), buffer, buffer + bytes_available);

            // Print received characters for debugging purposes
            std::cout << "Received: ";
            for (int i = 0; i < bytes_available; ++i) {
                std::cout << buffer[i];
            }
            std::cout << std::endl;
        }
        usleep(100000); // Short delay to prevent high CPU usage
    }
}

std::vector<char> UARTReceiver::getReceivedData() const {
    // Lock the mutex before accessing the received data vector
    std::lock_guard<std::mutex> lock(data_mutex_);
    return received_data_;
}
