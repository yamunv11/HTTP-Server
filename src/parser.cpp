#include "parser.h"
#include <sstream>
#include <vector>

int read_token(std::istringstream &ss, std::string &token)
{
    constexpr int IN = 1;
    constexpr int OUT = 0;
    char c = '\0';
    token = "";
    bool state = IN;
    while (ss.get(c)) {
        if (c == ' ' || c == '\n' || c == '\r') {
            if (state == OUT) {
                token.push_back(c);
            } else if (state == IN && token.size() == 0) {
                token.push_back(c);
                state = OUT;
            } else {
                ss.putback(c);
                break;
            }
        } else {
            if (state == IN) {
                token.push_back(c);
            } else {
                ss.putback(c);
                break;
            }
        }
    }
    if (ss.eof())
        return -1;
    if (token.size() == 0)
        return 0;
    return 1;
}

request_t parse_request(const std::string &raw)
// only handles requests with no body, doesn't keep track of headers, only for method and paths
{
    request_t r;
    std::istringstream ss { raw };
    std::string token;
    std::vector<std::string> tokens;
    int n;
    while ((n = read_token(ss, token)) != -1) {
        if (token == "\r\n\r\n")
            break;
        if (n > 0)
            tokens.push_back(token);
    }
    r.method = tokens[0];
    r.path = tokens[2];
    r.version = tokens[4];
    return r;
}
