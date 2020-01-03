#include "./server/madn_server.hpp"

int main (int argc, char* argv[]) {
    if (argc > 1) {
        std::string filePath(argv[1]);
        ServerMadn server(filePath);
        server.startServer(8999);
        return 0x00;
    } else {
        ServerMadn server;
        server.startServer(8999);
        return 0x00;
    }
}