#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML\System\Sleep.hpp>
#include "../../view/view.h"
#include "../../view/config.h"
#include "madn_client.hpp"

ClientMadn::ClientMadn() {
}

//ClientMadn ClientMadn::GetClient()
//{
//    return *this;
//}

std::string getCurrentDateWithTime_client () {
    auto currentTime_t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    char stringDateBuffer[100] = {0};
    tm timeinfo;
    localtime_s(&timeinfo, &currentTime_t);
    std::strftime(stringDateBuffer, sizeof(stringDateBuffer), "%Y-%m-%d %H:%M:%S", &timeinfo);
    //std::strftime(stringDateBuffer, sizeof(stringDateBuffer), "%Y-%m-%d %H:%M:%S", std::localtime(&currentTime_t)); //Die beiden Zeilen darüber ersetzen diese Funktion auch threadsafe
    return stringDateBuffer;
}

void ClientMadn::readHandler2 (connection_madn_ptr connection_ptr) {
    std::cout << "Log[" << getCurrentDateWithTime_client() << "]: " << "Server answer: " << connection_ptr->connection_buffer << std::endl;
    auto boostBuffer = boost::asio::buffer(connection_ptr->connection_buffer, 100);
            boost::asio::async_read(connection_ptr->socket, boostBuffer, [this, connection_ptr](boost::system::error_code errorCode1, size_t length){
                if (!errorCode1) this->readHandler1(connection_ptr);
            });
}

void ClientMadn::writeHandler (connection_madn_ptr connection_ptr) {
    std::cout << "Log[" << getCurrentDateWithTime_client() << "]: " << "Wait for reaction!" << std::endl;
    auto boostBuffer = boost::asio::buffer(connection_ptr->connection_buffer, 100);
    boost::asio::async_read(connection_ptr->socket, boostBuffer, [this, connection_ptr](errorCode errorCode, size_t length){
        if (!errorCode) this->readHandler2(connection_ptr);
    });
}

void ClientMadn::readHandler1 (connection_madn_ptr connection_ptr) {
    std::cout << "Content: " << connection_ptr->connection_buffer << std::endl;
    std::cout << "Enter your answer: ";
    std::string answer;
    std::getline(std::cin, answer);
    if (answer.compare("exit") == 0) return;
    char buffer[100];
    strcpy_s(buffer, answer.c_str());
    auto boostBuffer = boost::asio::buffer(buffer);
    boost::asio::async_write(connection_ptr->socket, boostBuffer, [this, connection_ptr](errorCode errorCode, size_t length){
        if (!errorCode) this->writeHandler(connection_ptr);
    });
}

void ClientMadn::startClient (std::string ip, std::string port) {
    std::cout << "Log[" << getCurrentDateWithTime_client() << "]: " << "Client started." << std::endl;
    boost::asio::io_service io_service;
    connection_ptr = std::make_shared<connection_madn>(io_service);


    tcp::resolver resolver{io_service};
    tcp::resolver::query query(ip, port);
    auto it = resolver.resolve(query);

    playerNumber = StipulatePlayerNumber();


   /* auto connectionHandler = [this, connection_ptr](const boost::system::error_code& errorCode, auto x){
        if (!errorCode) {
            auto boostBuffer = boost::asio::buffer(connection_ptr->connection_buffer, 100);
            boost::asio::async_read(connection_ptr->socket, boostBuffer, [this, connection_ptr](boost::system::error_code errorCode1, size_t length){
                if (!errorCode1) this->readHandler1(connection_ptr);
            });
        }
    };

    boost::asio::async_connect(connection_ptr->socket, it, connectionHandler);

    auto t = std::thread([&io_service]() {io_service.run();});
    t.join();
    */
}

void ClientMadn::send(std::string message)
{
    std::cout << "Log[" << getCurrentDateWithTime_client() << "]: " << "Send:" << message << std::endl;
    char buffer[100];
    strcpy_s(buffer, message.c_str());
    auto boostBuffer = boost::asio::buffer(buffer);
    //auto boostBuffer = boost::asio::buffer(connection_ptr->connection_buffer, 100);
    /*boost::asio::async_read(connection_ptr->socket, boostBuffer, [this, connection_ptr](errorCode errorCode, size_t length) {
        if (!errorCode) this->readHandler2(connection_ptr);
        });*/
}

int ClientMadn::StipulatePlayerNumber()
{
    std::string answer = "";
    int number = 0;
    while (answer != ERRORANSWERFROMSERVER && number <= 4)
    {
        ++number;
        send(number + "join");
        answer = AwaitServerResponse();
    }
    if (answer == ERRORANSWERFROMSERVER)
    {
        std::cout << "Konnte keine Verbindung aufbauen, versuche es in 5 Sekunden erneut."; 
        sf::sleep(sf::seconds(5));
        return StipulatePlayerNumber();
    }
    else
    {
        return number;
    }
}

std::string ClientMadn::AwaitServerResponse()
{
    return std::string();
}

void ClientMadn::IncomingMessage(std::string message)
{
    if (message.length() > 62) 
    {
        view.setPositions(message);
        if (message.at(62) != '0')
        {
            aktiv = false;
        }
    }
    else
        std::cout << "Erhaltene Nachricht zu kurz.";
}

void ClientMadn::MessageHandler()
{
    while (aktiv)
    {
        //TODO
        // Nachricht an IncommingMessage Übergeben
    }
}

void ClientMadn::RunGame()
{
    aktiv = true;
    MessageHandler(); //TODO in Thread auslagern
}

void ClientMadn::SendStart()
{
    send(playerNumber + "start");
}

void ClientMadn::SendMeepleClicked(int player, int meeple)
{
    if (player == playerNumber)
    {
        send(playerNumber + "" + meeple);
    } //TODO evtl. Fehlermeldung wenn falsche angeklickt
}

void ClientMadn::SendRollTheDice()
{
    send(playerNumber + "D");
}

void ClientMadn::SendPass()
{
    send(playerNumber + "P");
}

void ClientMadn::SendQuit()
{
    send(playerNumber + "Q");
}

int client_main () {
    ClientMadn client;
    client.startClient("localhost", "8999");
    sf::RenderWindow window(sf::VideoMode(PLAYGROUNDINTERVAL * 11 + SIDEPANEL, PLAYGROUNDINTERVAL * 11), "Mensch ärgere dich nicht");
    View view = InitializeView();
    window.setFramerateLimit(30);

    client.RunGame();

    std::cout << "Log[" << getCurrentDateWithTime_client() << "]: " << "Client stopped." << std::endl;
    return 0x00;
}

//TODO: Wenn Spiel beendet (unterbrochen) ohne Sieger, aktiv auf false setzen