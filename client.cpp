#include <iostream>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <thread>


void receive_messages(int client_fd)
{
    char buffer[1024]{};

    while (true)
    {
        ssize_t bytes_received = recv(client_fd, buffer, sizeof(buffer) - 1, 0);

        if (bytes_received > 0)
        {
            buffer[bytes_received] = '\0';
            std::cout << buffer << '\n';
        }
        else
        {
            break;
        }
    }
}


int main()
{
    std::cout << "Client starting...\n";

    std::string username;

    std::cout << "Enter username: ";
    std::getline(std::cin, username);

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

    // Send the username as the first message to the server.
    send(client_fd, username.c_str(), username.size(), 0);

    // Receive server messages in a separate thread.
    std::thread receive_thread(receive_messages, client_fd);
    receive_thread.detach();

    std::string message;


    while (true)
    {
        std::getline(std::cin, message);

        if (message == "/quit")
        {
            break;
        }

        // Send the typed message to the server.
        send(client_fd, message.c_str(), message.size(), 0);
    }

    close(client_fd);
    

    return 0;
}