#include "server.h"
#include <arpa/inet.h>
#include <exception>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>

int main()
try {
    Server server;
    while (1) {
        client client = server.accept_connection();
        char buffer[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &client.ip_addr, buffer, INET_ADDRSTRLEN);
        std::cout << "=======================================\n";
        std::cout << "IP: " << buffer << "\nConnection accepted\n\n";

        std::string request = server.recieve(client);
        std::cout << "Client sent:\n"
                  << request << '\n';

        server.respond(client, htos("/home/amun/projects/blog/site/index.html"));
        std::cout << "Response sent\n";
        std::cout << "=======================================\n";
    }
    return 0;
} catch (std::exception &e) {
    std::cout << e.what() << '\n';
}
