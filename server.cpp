#include <iostream>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>


int main()
{
    std::cout << "Server starting...\n";

    // Create an IPv4 TCP socket.
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);

    if (server_fd == -1)
    {
        std::cerr << "Failed to create socket\n";
        return 1;
    }

    sockaddr_in server_address{};

    // Use IPv4 addresses.
    server_address.sin_family = AF_INET;

    // Use port 8080.
    server_address.sin_port = htons(8080);

    // Accept connections on any local network interface.
    server_address.sin_addr.s_addr = INADDR_ANY;

    if (bind(server_fd, reinterpret_cast<sockaddr*>(&server_address), sizeof(server_address)) == -1)
    {
        std::cerr << "Failed to bind socket\n";
        return 1;
    }

    std::cout << "Socket bound to port 8080\n";

    // Start listening for incoming TCP connections.
    if (listen(server_fd, 5) == -1)
    {
        std::cerr << "Failed to listen on socket\n";
        return 1;
    }

    // Wait for a client and create a socket for that connection.
    int client_fd = accept(server_fd, nullptr, nullptr);

    if (client_fd == -1)
    {
        std::cerr << "Failed to accept client\n";
        return 1;
    }

    std::cout << "Client connected\n";

    // Receive one message from the connected client.
    char buffer[1024]{};


    while (true)
    {
        ssize_t bytes_received = recv(client_fd, buffer, sizeof(buffer) - 1, 0);

        if (bytes_received > 0)
        {
            buffer[bytes_received] = '\0';
            std::cout << "Client says: " << buffer << '\n';
        }
    }

    close(client_fd);
    close(server_fd);

    return 0;
}