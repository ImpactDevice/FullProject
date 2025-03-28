#include "UARTReceiver.h"
#include <iostream>
#include <chrono>
#include <thread>
#include <vector>

int main() {
    try {
        UARTReceiver receiver("/dev/ttyS0", 38400, 1, 'E');

        std::cout << "Starting UART receiver..." << std::endl;
        receiver.startReceiving();

        // Let it receive data for 10 seconds
        std::this_thread::sleep_for(std::chrono::seconds(10));

        std::cout << "Stopping UART receiver..." << std::endl;
        receiver.stopReceiving();

        // Get and print the received data
        std::vector<char> data = receiver.getReceivedData();
        std::cout << "Received data: ";
        for (char c : data) {
            std::cout << c;
        }
        std::cout << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
