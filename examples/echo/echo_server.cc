#include "rpc/server.h"
#include <iostream>

int main() {
#ifdef RPCLIB_USE_LOCAL_SOCKETS
    rpc::server srv("/tmp/echo-socket");
    std::cout << "registered on  port /tmp/echo-socket\n";
#else
    rpc::server srv(rpc::constants::DEFAULT_PORT);
    std::cout << "registered on  port " << srv.port() << std::endl;
#endif

    srv.bind("echo", [](std::string const& s) {
        return s;
    });

    srv.run();
    return 0;
}
