#ifndef SERVER_H
#define SERVER_H

#include <netinet/in.h>
#include <string>
#include <unistd.h>

class client {
public:
    client() : client_socket { 0 } , ip_addr { 0 } { }
    inline ~client() { close(client_socket); };
    int client_socket;
    uint32_t ip_addr;

    void close_connection();
};

class Server {
    int server_socket;
    struct sockaddr_in sa;

public:
    Server();
    ~Server();

    client acpt_con();
    std::string recieve(client &c);
    void respond(client &c, std::string response);
};

std::string htos(std::string);

#endif /* SERVER_H */
