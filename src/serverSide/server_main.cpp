#include "./server/madn_server.hpp"

int main () {
    ServerMadn server;
    server.startServer(8999);
    return 0x00;
}