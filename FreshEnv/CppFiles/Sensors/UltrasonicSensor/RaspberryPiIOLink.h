#ifndef RASPBERRY_PI_IOLINK_H
#define RASPBERRY_PI_IOLINK_H

#include <string>
#include <thread>
#include <atomic>

class RaspberryPiIOLink {
public:
    RaspberryPiIOLink(const std::string& device, int baudRate);
    ~RaspberryPiIOLink();

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

#endif // RASPBERRY_PI_IOLINK_H