#include "./client/madn_client.hpp"
#include "../view/view.hpp"
#include "./client/boardClient/madn_client_board.hpp"

void startUserClient (ClientMadnPtr client, std::string ip, std::string port) {
    client->startClient(ip, port);
}

void startBoardClient (ClientMadnBoardPtr client, std::string ip, std::string port) {
    client->startClient(ip, port);
}

int main () {
    ClientMadnPtr client = std::make_shared<ClientMadn>();

    std::string ip;
    std::string port;

    std::cout << "Zu welcher IP willst du dich verbinden?" << std::endl;
    std::cin >> ip;
    std::cout << "Auf welchem Port läuft das Spiel?" << std::endl;
    std::cin >> port;
    
    std::thread clientThread (startUserClient, client, ip, port);

    ViewPtr view = InitializeView(client);

    ClientMadnBoardPtr boardClient = std::make_shared<ClientMadnBoard>(view);
    
    std::thread boardClientThread (startBoardClient, boardClient, ip, port);

    sf::RenderWindow window(sf::VideoMode(PLAYGROUNDINTERVAL * 11 + SIDEPANEL, PLAYGROUNDINTERVAL * 11), "Mensch ärgere dich nicht");
    window.setFramerateLimit(30);

    RunView(window, view);

    clientThread.join();
    boardClientThread.join();

    return 0x00;
}