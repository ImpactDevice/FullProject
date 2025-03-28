#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#define BUFFER_SIZE 1024  // Adjust based on your data stream size
#define PORT 5000         // Ensure this matches the sender's port

int main() {
    // Create a socket
    int udpSocket = socket(AF_INET, SOCK_DGRAM, 0);
    if (udpSocket < 0) {
        std::cerr << "Error: Could not create socket" << std::endl;
        return -1;
    }

    // Bind the socket to a specific port
    sockaddr_in serverAddr;
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;  // Listen on all available interfaces
    serverAddr.sin_port = htons(PORT);

    if (bind(udpSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        std::cerr << "Error: Could not bind to port " << PORT << std::endl;
        close(udpSocket);
        return -1;
    }

    std::cout << "UDP server is listening on port " << PORT << "..." << std::endl;

    // Loop to receive data
    char buffer[BUFFER_SIZE];
    sockaddr_in clientAddr;
    socklen_t clientAddrLen = sizeof(clientAddr);

    while (true) {
        // Receive data from the sender
        ssize_t bytesRead = recvfrom(udpSocket, buffer, BUFFER_SIZE - 1, 0, (sockaddr*)&clientAddr, &clientAddrLen);
        if (bytesRead < 0) {
            std::cerr << "Error: Failed to receive data" << std::endl;
            continue;
        }

        buffer[bytesRead] = '\0';  // Null-terminate the received data
        std::cout << "Received " << bytesRead << " bytes: " << buffer << std::endl;
    }

    // Close the socket (this won't be reached unless you exit the loop)
    close(udpSocket);
    return 0;
}
