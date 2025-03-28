#ifndef RASPBERRY_PI_SERIAL_H
#define RASPBERRY_PI_SERIAL_H

#include <string>
#include <thread>
#include <atomic>

class RaspberryPiSerial {
public:
    RaspberryPiSerial(const std::string& device, int baudRate);
    ~RaspberryPiSerial();

    void start();
    void stop();
    std::string getData() const;

private:
    void run();

    std::string device;
    int baudRate;
    std::atomic<bool> running;
    std::thread workerThread;
    std::string data;
};

#endif // RASPBERRY_PI_SERIAL_H