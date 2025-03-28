#ifndef UARTRECEIVER_H
#define UARTRECEIVER_H

#include <string>
#include <thread>
#include <atomic>
#include <vector>
#include <mutex>

class UARTReceiver {
public:
    UARTReceiver(const std::string& device, int baud_rate, int stop_bits=1, char parity='E');
    ~UARTReceiver();

    void startReceiving();
    void stopReceiving();
    std::vector<char> getReceivedData() const;

private:
	void setupSerialPort();
    void receiveData();
    
    std::string device_;
    int baud_rate_;
    int serial_port_;
    std::thread receive_thread_;
    std::atomic<bool> running_;
    std::vector<char> received_data_;
    mutable std::mutex data_mutex_;
    
    int stop_bits_;
    char parity_;
};

#endif // UARTRECEIVER_H
