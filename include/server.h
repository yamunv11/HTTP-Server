#ifndef SERVER_H
#define SERVER_H

#include <netinet/in.h>
#include <string>
#include <unistd.h>

class client_t {
public:
    client_t() : client_socket { 0 } , ip_addr { 0 } { }
    inline ~client_t() { close(client_socket); };
    int client_socket;
    uint32_t ip_addr;

    void close_connection();
};

class server_t {
    int server_socket;
    struct sockaddr_in sa;

public:
    server_t();
    ~server_t();

    client_t accept_connection();
    std::string recieve(client_t &c);
    void respond(client_t &c, const std::string &response);
};

std::string htos(std::string);

#endif /* SERVER_H */
