#include "server.h"
#include <arpa/inet.h>
#include <exception>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>

int main()
try {
    Server s;
    while (1) {
        client c = s.acpt_con();
        char buffer[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &c.ip_addr, buffer, INET_ADDRSTRLEN);
        std::cout << "=======================================\n";
        std::cout << "IP: " << buffer << "\nConnection accepted\n\n";

        std::string request = s.recieve(c);
        std::cout << "Client sent:\n"
                  << request << '\n';

        s.respond(c, htos("/home/amun/projects/blog/site/index.html"));
        std::cout << "Response sent\n";
        std::cout << "=======================================\n";
    }
    return 0;
} catch (std::exception &e) {
    std::cout << e.what() << '\n';
}
