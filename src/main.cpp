#include <iostream>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include "server.h"

const std::string message = "HTTP/1.1 200 OK\r\n"
                            "Content-Type: text/plain\r\n"
                            "Connection: close\r\n"
                            "\r\n"
                            "Hello from server\r\n";

int main()
{
    Server s;
    while (1) {
        client c = s.acpt_con();
        char buffer[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &c.ip_addr, buffer, INET_ADDRSTRLEN);
        std::cout << "=======================================\n";
        std::cout << "IP: " << buffer << "\nConnection accepted\n\n";

        std::string request = s.recieve(c);
        std::cout << "Client sent:\n" << request << '\n';

        s.respond(c, htos("/home/amun/projects/blog/site/index.html"));
        std::cout << "Response sent\n";
        std::cout << "=======================================\n";
    }
    return 0;
}
