#include "madn_server_logic.hpp"

// Arguments are used, if you want to load a save game.
int main (int argc, char* argv[]) {
    if (argc > 1) {
        std::cout << "Load savegame." << std::endl;
    } else {
        std::cout << "New game" << std::endl;
        ServerMadn server;
        MenschAergereDichNichtGame game;

        server.startServer(8999);
    }
}