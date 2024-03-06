#include "rpc/client.h"

#include <iostream>

int main() {
#ifdef RPCLIB_USE_LOCAL_SOCKETS
    rpc::client c("/tmp/echo-socket");
#else
    rpc::client c("localhost", rpc::constants::DEFAULT_PORT);
#endif

    std::string text;
    while (std::getline(std::cin, text)) {
        if (!text.empty()) {
            std::string result(c.call("echo", text).as<std::string>());
            std::cout << "> " <<  result << std::endl;
        }
    }
}
