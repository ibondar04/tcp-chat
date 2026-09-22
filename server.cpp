#include <iostream>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <thread>
#include <vector>
#include <mutex>


std::vector<int> clients;
std::mutex clients_mutex;


void remove_client(int client_fd)
{
    std::lock_guard<std::mutex> lock(clients_mutex);

    for (auto it = clients.begin(); it != clients.end(); ++it)
    {
        if (*it == client_fd)
        {
            clients.erase(it);
            break;
        }
    }
}


void handle_client(int client_fd)
{
    std::cout << "Client connected\n";

    char buffer[1024]{};

    // Receive the client's username first.
    ssize_t username_bytes = recv(client_fd, buffer, sizeof(buffer) - 1, 0);

    if (username_bytes <= 0)
    {
        std::cerr << "Failed to receive username\n";
        remove_client(client_fd);
        close(client_fd);
        return;
    }

    buffer[username_bytes] = '\0';
    std::string username = buffer;

    // Receive messages from the connected client.
    while (true)
    {
        ssize_t bytes_received = recv(client_fd, buffer, sizeof(buffer) - 1, 0);

        if (bytes_received > 0)
        {
            buffer[bytes_received] = '\0';

            std::string full_message = username + ": " + buffer;
            std::cout << full_message << '\n';

            {
                std::lock_guard<std::mutex> lock(clients_mutex);

                // Send the message to every other connected client.
                for (int other_client_fd : clients)
                {
                    if (other_client_fd != client_fd)
                    {
                        ssize_t bytes_sent = send(other_client_fd, full_message.c_str(), full_message.size(), 0);

                        if (bytes_sent == -1)
                        {
                            std::cerr << "Failed to send message to client\n";
                        }
                    }
                }
            }
        }
        else if (bytes_received == 0)
        {
            std::cout << "Client disconnected\n";
            break;
        }
        else
        {
            std::cerr << "Failed to receive message\n";
            break;
        }
    }
    
    remove_client(client_fd);
    close(client_fd);
}


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


    while (true)
    {
        // Wait for a client and create a socket for that connection.
        int client_fd = accept(server_fd, nullptr, nullptr);

        if (client_fd == -1)
        {
            std::cerr << "Failed to accept client\n";
            return 1;
        }

        {
            std::lock_guard<std::mutex> lock(clients_mutex);
            clients.push_back(client_fd);
        }

        std::thread client_thread(handle_client, client_fd);
        client_thread.detach();
    }
    
    close(server_fd);
    return 0;
}