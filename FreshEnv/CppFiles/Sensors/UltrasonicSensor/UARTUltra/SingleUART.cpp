#include <iostream>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <chrono>
#include <cstring>
#include <errno.h>
#include <poll.h>
#include <string.h>

//Physical pin 7: Tx
//Physical Pin 29: Rx
//GPIO.7
//GPIO.21


//might want to change these

//IGNPAR ignore parity errors



int configureUart(int fd, int baudRate, int stopBits, int parity, bool invertSignal) {
    struct termios tty;

    if (tcgetattr(fd, &tty) != 0) {
        std::cerr << "Error getting tty attributes" << std::endl;
        return -1;
    }

    cfsetospeed(&tty, baudRate);
    cfsetispeed(&tty, baudRate);

    tty.c_cflag &= ~CSIZE;
    tty.c_cflag |= CS8;  // 8-bit characters
    
    tty.c_cflag |= CREAD;
    tty.c_cflag |= CLOCAL;

    // Setting parity
    tty.c_cflag &= ~(PARENB | PARODD);
    if (parity == 0) {
        tty.c_cflag &= ~PARENB; // No parity
    } else if (parity == 1) {
        tty.c_cflag |= PARENB;  // Even parity
        tty.c_cflag &= ~PARODD;
    } else if (parity == 2) {
        tty.c_cflag |= PARENB;  // Odd parity
        tty.c_cflag |= PARODD;
    }

    // Setting stop bits
    if (stopBits == 1) {
        tty.c_cflag &= ~CSTOPB;  // 1 stop bit
    } else if (stopBits == 2) {
        tty.c_cflag |= CSTOPB;   // 2 stop bits
    }

    // Signal inversion (not directly supported by termios, so we use software inversion)
    //~ if (invertSignal) {
        //~ tty.c_iflag |= INPCK;
    //~ } else {
        //~ tty.c_iflag &= ~INPCK;
    //~ }
    
    tty.c_iflag &= ~(IXON | IXOFF | IXANY);
    tty.c_oflag &= ~OPOST;

    if (tcsetattr(fd, TCSANOW, &tty) != 0) {
        std::cerr << "Error setting tty attributes" << std::endl;
        close(fd);
        return -1;
    }

    return 0;
}



int writeUart(int fd, const char* data) {
	int dataSize = strlen(data);
	int bytesWritten = write(fd, data, dataSize);
	if (bytesWritten != dataSize) {
        std::cerr << "Error writing data to UART" << std::endl;
        return -1;
    }

    return 0;
}

int writeUartHex(int fd, const uint8_t* data, size_t dataSize) {
	int bytesWritten = write(fd, data, dataSize);
		if (bytesWritten != dataSize) {
        std::cerr << "Error writing data to UART" << std::endl;
        return -1;
    }
	return 0;
}


void printHex(const char* buffer, int length) {
	for (int i = 0; i < length; ++i) {
        printf("%02X ", static_cast<unsigned char>(buffer[i]));
    }
    printf("\n");
}

int main(int argc, char* argv[]) {
    const char* device = "/dev/ttyAMA3";  // Change this to the appropriate device
    int fd = open(device, O_RDWR | O_NOCTTY | O_NDELAY);
    
    if (fd == -1) {
        std::cerr << "Error opening " << device << std::endl;
        return -1;
    }

    int baudRate = B38400;
    int stopBits = 1;
    int parity = 1;
    bool invertSignal = false;

    if (configureUart(fd, baudRate, stopBits, parity, invertSignal) == -1) {
        close(fd);
        return -1;
    }



    
    //~ const char* message = 0x94;
    //~ if (writeUart(fd, message) == -1) {
        //~ close(fd);
        //~ std::cout << "Failed To write Message" << std::endl;
        //~ return -1;
    //~ }
    
    //~ uint8_t message[] = {0xFF};
    //~ size_t messageSize = sizeof(message) / sizeof(message[0]);
    
    //~ if (writeUartHex(fd, message, messageSize) == -1) {
		//~ close(fd);
		//~ std::cerr << "Failed to Write: " << message << std::endl;
		//~ return -1;
	//~ }
	
    
    
    
    auto start = std::chrono::steady_clock::now();
    char buffer[18];
    int bytesRead;
    
    //~ while (true) {
		//~ std::cout << "About to Read()" << std::endl;
        //~ bytesRead = read(fd, &buffer, sizeof(buffer) - 1);
        //~ std::cout << "Read() returned: " << bytesRead << std::endl;
        //~ std::cout << "Buffer is: " << buffer << std::endl;
        //~ if (bytesRead > 0) {
            //~ buffer[bytesRead] = '\0';
            //~ std::cout << buffer;
        //~ }
        //~ if (bytesRead == -1) {
			//~ std::cerr << "Error Reading UART " << strerror(errno) << std::endl;
		//~ };

        //~ auto now = std::chrono::steady_clock::now();
        //~ auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - start).count();

        //~ if (elapsed >= 1) {
            //~ break;
        //~ }
    //~ }
    // Your UART communication code goes here


    //~ struct pollfd pfd;
    //~ pfd.fd = fd;
    //~ pfd.events = POLLIN;

    //~ while (true) {
        //~ int pollResult = poll(&pfd, 1, 10);  // Poll with a timeout of 1000 milliseconds

        //~ if (pollResult == -1) {
            //~ std::cerr << "Poll error: " << strerror(errno) << std::endl;
            //~ break;
        //~ } else if (pollResult == 0) {
            //~ std::cerr << "Poll timeout" << std::endl;
        //~ } else {
            //~ if (pfd.revents & POLLIN) {
                //~ bytesRead = read(fd, &buffer, sizeof(buffer) - 1);
                //~ if (bytesRead == -1) {
                    //~ std::cerr << "Read error: " << strerror(errno) << std::endl;
                    //~ break;
                //~ }
                //~ std::cout << bytesRead << std::endl;
                //~ if (bytesRead > 0) {
					//~ std::cout << "Print prior to buffer" << std::endl;
					//~ printHex(buffer, sizeof(buffer));
                    //~ buffer[bytesRead] = '\0';
                    //~ std::cout << buffer;
                //~ }
            //~ }
        //~ }

        //~ auto now = std::chrono::steady_clock::now();
        //~ auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - start).count();

        //~ if (elapsed >= 0.01) {
            //~ break;
        //~ }
    //~ }
    
    //Select
	while (true) {
		fd_set readfds;
		FD_ZERO(&readfds);
		FD_SET(fd, &readfds);

		struct timeval timeout;
		timeout.tv_sec = 10;  // Timeout of 1 second
		timeout.tv_usec = 0;

		int selectResult = select(fd + 1, &readfds, NULL, NULL, &timeout);

		if (selectResult == -1) {
			std::cerr << "Select error: " << strerror(errno) << std::endl;
			break;
		} else if (selectResult == 0) {
			std::cerr << "Select timeout" << std::endl;
		} else {
			if (FD_ISSET(fd, &readfds)) {
				bytesRead = read(fd, &buffer, sizeof(buffer) - 1);
				if (bytesRead == -1) {
					std::cerr << "Read error: " << strerror(errno) << std::endl;
					break;
				}
				std::cout << bytesRead << std::endl;
				if (bytesRead > 0) {
					std::cout << "Print prior to buffer" << std::endl;
					printHex(buffer, sizeof(buffer));
					buffer[bytesRead] = '\0';
					std::cout << buffer;
				}
			}
		}

		auto now = std::chrono::steady_clock::now();
		auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - start).count();

		if (elapsed >= 1) {
			break;
		}
	}



    close(fd);
    return 0;
}
