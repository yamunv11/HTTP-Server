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
#define PORT 6969

void client_t::close_connection()
{
    close(client_socket);
}

server_t::server_t()
{
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1)
        cerror("opening socket failed");
    sa.sin_family = AF_INET;
    sa.sin_port = htons(PORT);
    sa.sin_addr.s_addr = INADDR_ANY;
    int opt = 1;
    setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    setsockopt(server_socket, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt));
    if (bind(server_socket, (struct sockaddr *)&sa, sizeof(sa)) < 0)
        cerror("bind failed");
    if (listen(server_socket, QUEUE) == -1)
        cerror("listen failed");
    std::cout << "Server initialized\nlistenting on port 6969...\n";
}

server_t::~server_t()
{
    close(server_socket);
}

client_t server_t::accept_connection()
{
    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);
    int client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_len);
    if (client_socket == -1) {
        throw std::runtime_error(std::string("Accept failed: ") + strerror(errno));
    }

    client_t client;
    client.client_socket = client_socket;
    client.ip_addr = client_addr.sin_addr.s_addr;

    return client;
}

std::string server_t::recieve(client_t &c)
// only handles requests with no body for now
// returns a string, so not handling for images, etc.
{
    char buffer[256] = { 0 };
    std::string request;
    ssize_t n;
    while (1) {
        n = recv(c.client_socket, buffer, sizeof(buffer) - 1, 0);
        if (n < 0) cerror("recv failed");
        if (n == 0) break;

        request.append(buffer, n);

        if (request.find("\r\n\r\n") != std::string::npos)
            break;
    }
    return request;
}

void server_t::respond(client_t &c, const std::string &response)
{
    const char *message = response.c_str();
    size_t total = 0;
    size_t len = strlen(message);
    while (total < len) {
        ssize_t n;
        if ((n = send(c.client_socket, message + total, len - total, 0)) == -1)
            cerror("send failed");
        total += n;
    }
}

std::string htos(std::string s)
{
    std::ifstream f(s);
    if (!f.is_open())
        return "";

    const std::string headers = "HTTP/1.1 200 OK\r\n"
                                "Content-Type: text/html\r\n"
                                "Connection: close\r\n"
                                "\r\n";
    std::ostringstream ss;
    ss << f.rdbuf();
    std::string html = ss.str();
    return headers + html + "\r\n";
}
