#include <iostream>
#include <sys/socket.h>
#include <unistd.h>


int main()
{
    std::cout << "Server starting...\n";

    // Create an IPv4 TCP socket.
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);

    return 0;
}