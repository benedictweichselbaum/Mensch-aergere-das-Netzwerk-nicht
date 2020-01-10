#pragma once

#include <SFML/Graphics.hpp>

#include "../../client_server_connection/connection.hpp"
#include "../../view//view.h"

#define ERRORANSWERFROMSERVER "refused" // Antwort wenn Spielernummer vergeben

using tcp = boost::asio::ip::tcp;
using errorCode = boost::system::error_code;

std::string getCurrentDateWithTime_client ();

class ClientMadn {
    private:
        boost::asio::io_service io_service;
        connection_madn_ptr connection_ptr;
        int playerNumber;
        sf::RenderWindow window;
        View view;
        bool aktiv;
    public:
        ClientMadn ();
       // ClientMadn GetClient();
        void readHandler2 (connection_madn_ptr connection_ptr);
        void writeHandler (connection_madn_ptr connection_ptr);
        void readHandler1 (connection_madn_ptr connection_ptr);
        // void connectionHandler (connection_madn_ptr connection_ptr);
        void startClient (std::string ip, std::string port);
        void send(std::string message);
        int StipulatePlayerNumber();
        std::string AwaitServerResponse();
        void IncomingMessage(std::string message);
        void MessageHandler();
        void RunGame();
        void SendStart();
        void SendMeepleClicked(int player, int meeple);
        void SendRollTheDice();
        void SendPass();
        void SendQuit();
};
