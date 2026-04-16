#include "server.h"
#include "utils.h"
#include <cstdio>
#include <cstring>
#include <fstream>
#include <iostream>
#include <netinet/in.h>
#include <sstream>
#include <stdexcept>
#include <sys/socket.h>
#include <unistd.h>

#define QUEUE 5
#define PORT 8080

void client::close_connection()
{
    close(client_socket);
}    


Server::Server()
{
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1)
        error("opening socket failed");
    sa.sin_family = AF_INET;
    sa.sin_port = htons(PORT);
    sa.sin_addr.s_addr = INADDR_ANY;
    int opt = 1;
    setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    setsockopt(server_socket, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt));
    if (bind(server_socket, (struct sockaddr *)&sa, sizeof(sa)) < 0)
        error("bind failed");
    if (listen(server_socket, QUEUE) == -1)
        error("listen failed");
    std::cout << "Server initialized\nready to accept connections...\n";
}

Server::~Server()
{
    close(server_socket);
}

client Server::acpt_con()
{
    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);
    int client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_len);
    if (client_socket == -1) {
        throw std::runtime_error(std::string("Accept failed: ") + strerror(errno));
    }

    client client;
    client.client_socket = client_socket;
    client.ip_addr = client_addr.sin_addr.s_addr;

    return client;
}

std::string Server::recieve(client &c)
// only handles requests with no body for now
{
    char buffer[256] = {0};
    std::string request;
    ssize_t n;
    while (1) {
        n = recv(c.client_socket, buffer, sizeof(buffer) - 1, 0);
        if (n < 0) error("recv failed");
        if (n == 0) break;

        request.append(buffer, n);

        if (request.find("\r\n\r\n") != std::string::npos)
            break;
    }
    return request;
}

void Server::respond(client &c, std::string response)
{
    const char *message = response.c_str();
    size_t total = 0;
    size_t len = strlen(message);
    while (total < len) {
        ssize_t n;
        if ((n = send(c.client_socket, message + total, len - total, 0)) == -1)
            error("send failed");
        total += n;
    }
}

std::string htos(std::string s)
{
    const std::string message = "HTTP/1.1 200 OK\r\n"
                                "Content-Type: text/html\r\n"
                                "Connection: close\r\n"
                                "\r\n";
    std::ifstream f(s);
    if (!f.is_open())
        throw std::runtime_error("Can't open file: " + s);
    
    std::ostringstream ss;
    ss << f.rdbuf();
    std::string html = ss.str();
    return message + html + "\r\n";
}    
