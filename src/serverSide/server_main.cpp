#include "./server/madn_server.hpp"

int server_main () {
    ServerMadn server;
    server.startServer(8999);
    return 0x00;
}