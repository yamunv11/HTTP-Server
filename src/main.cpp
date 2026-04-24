#include "server.h"
#include "parser.h"
#include <arpa/inet.h>
#include <exception>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>

const std::string err404 = "HTTP/1.1 404 Not Found\r\n"
                           "Content-Type: text/html\r\n"
                           "Connection: close\r\n"
                           "\r\n"
                           "<html>\r\n"
                           "<head><title>404 Not Found</title></head>\r\n"
                           "<body>\r\n"
                           "<h1>Not Found</h1>\r\n"
                           "<h2>Skill issue loser</h2>\r\n"
                           "<p>The requested resource was not found on this server.</p>\r\n"
                           "</body>\r\n"
                           "</html>\r\n\r\n";


int main()
try {
    server_t server;
    while (1) {
        client_t client = server.accept_connection();
        char buffer[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &client.ip_addr, buffer, INET_ADDRSTRLEN);
        std::cout << "=======================================\n";
        std::cout << "IP: " << buffer << "\nConnection accepted\n\n";

        std::string req_str = server.recieve(client);
        request_t req = parse_request(req_str);

        if (req.path == "/") {
            std::string response = htos("/home/amun/projects/blog/site/index.html");
            server.respond(client, response);
        } else {
            std::string response = htos("/home/amun/projects/blog/site" + req.path);
            if (response.empty()) {
                server.respond(client, err404);
            }
            server.respond(client, response);
        }
        std::cout << "Response sent\n";
    }
    return 0;
} catch (std::exception &e) {
    std::cout << e.what() << '\n';
}
