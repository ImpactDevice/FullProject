#include <iostream>
#include <libserialport.h>

int main() {
    struct sp_port *port;
    sp_return result = sp_get_port_by_name("/dev/ttyACM0", &port);
    std::cout << result;

    if (result != SP_OK) {
        std::cerr << "Error finding port" << std::endl;
        return 1;
    }

    result = sp_open(port, SP_MODE_READ_WRITE);
    if (result != SP_OK) {
        std::cerr << "Error opening port" << std::endl;
        return 1;
    }

    // Example command to read distance measurement
    //const char *command = "READ_DISTANCE";
    //sp_nonblocking_write(port, command, strlen(command));

    char buffer[256];
    int bytes_read = sp_blocking_read(port, buffer, sizeof(buffer) - 1, 1000);
    if (bytes_read > 0) {
        buffer[bytes_read] = '\0'; // Null-terminate the string
        std::cout << "Data read: " << buffer << std::endl;
    } else {
        std::cerr << "Error reading from port" << std::endl;
    }

    sp_close(port);
    sp_free_port(port);

    return 0;
}
