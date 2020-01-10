#include "./client/madn_client.hpp"
#include "../view/view.hpp"

void func (ClientMadnPtr client) {
    client->startClient("localhost", "8999");
}

int main () {
    ClientMadnPtr client = std::make_shared<ClientMadn>();
    
    std::thread clientThread (func, client);

    ViewPtr view = InitializeView(client);

    sf::RenderWindow window(sf::VideoMode(PLAYGROUNDINTERVAL * 11 + SIDEPANEL, PLAYGROUNDINTERVAL * 11), "Mensch ärgere dich nicht");
    window.setFramerateLimit(30);

    RunView(window, view);

    return 0x00;
}