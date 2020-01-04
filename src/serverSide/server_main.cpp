#include "./server/madn_server.hpp"

// Server main method that starts a new server with the desired port.
// It lets you start a game from a save game file or just a default new game.
int main (int argc, char* argv[]) {
    ServerPtr server;
    if (argc > 1) {
        std::string saveGamePath(argv[1]);
        server = std::make_unique<ServerMadn>(saveGamePath);
    } else {
        server = std::make_unique<ServerMadn>();
    }

    unsigned short port;
    std::cout << "Auf welchem Port soll der Server laufen?:" << std::endl;
    std::cin >> port;
    server->startServer(port);

    return 0x00;
}