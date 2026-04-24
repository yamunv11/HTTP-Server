#ifndef PARSER_H
#define PARSER_H

#include <string>
#include <map>

struct Request {
    std::string method;
    std::string path;
    std::string version;
    std::map<std::string, std::string> headers;
    std::string body;
};

int read_token(std::istringstream &ss, std::string &token);
Request parse_request(const std::string &raw);

#endif  /* PARSER_H */
