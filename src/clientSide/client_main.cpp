#include "./client/madn_client.hpp"
#include "../view/view.hpp"

void func (ClientMadnPtr client, std::string ip, std::string port) {
    client->startClient("localhost", "8999");
}

int main () {
    ClientMadnPtr client = std::make_shared<ClientMadn>();

    std::string ip;
    std::string port;

    std::cout << "Zu welcher IP willst du dich verbinden?" << std::endl;
    std::cin >> ip;
    std::cout << "Auf welchem Port läuft das Spiel?" << std::endl;
    std::cin >> port;
    
    std::thread clientThread (func, client, ip, port);

    ViewPtr view = InitializeView(client);

    sf::RenderWindow window(sf::VideoMode(PLAYGROUNDINTERVAL * 11 + SIDEPANEL, PLAYGROUNDINTERVAL * 11), "Mensch ärgere dich nicht");
    window.setFramerateLimit(30);

    RunView(window, view);

    return 0x00;
}