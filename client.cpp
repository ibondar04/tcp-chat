#include <iostream>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>


int main()
{
    std::cout << "Client starting...\n";

    int client_fd = socket(AF_INET, SOCK_STREAM, 0);

    if (client_fd == -1)
    {
        std::cerr << "Failed to create socket\n";
        return 1;
    }

    sockaddr_in server_address{};

    // The server uses IPv4.
    server_address.sin_family = AF_INET;

    // The server is listening on port 8080.
    server_address.sin_port = htons(8080);

    // Convert the server IP address from text into network format.
    inet_pton(AF_INET, "127.0.0.1", &server_address.sin_addr);

    // Connect the client socket to the server.
    if (connect(client_fd,
                reinterpret_cast<sockaddr*>(&server_address),
                sizeof(server_address)) == -1)
    {
        std::cerr << "Failed to connect to server\n";
        return 1;
    }

    std::cout << "Connected to server\n";

    std::string message = "Hello from client";

    // Send one message to the server.
    send(client_fd, message.c_str(), message.size(), 0);

    return 0;
}