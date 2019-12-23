#pragma once

#include "../connection/connection.hpp"

using tcp = boost::asio::ip::tcp;

/* MADN stands for 'Mensch aergere dich nicht!' */

class ServerMadn {
    private:
        boost::asio::io_service io_service;
        tcp::acceptor acceptor;
        std::unique_ptr<std::thread> thread_ptr;
    public:
        ServerMadn ();
        void handlePlayerInput (connection_madn_ptr con_ptr);
        void getClientPlayerInput (connection_madn_ptr con_ptr);
        void welcomeNewClient (connection_madn_ptr con_ptr);
        void handleAcceptedConnection (connection_madn_ptr con_ptr);
        void startServer (ushort port);
};
