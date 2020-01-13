#ifndef H_MADNCLIENTB
#define H_MADNCLIENTB

#include <memory>
#include <iostream>
#include "../../../client_server_connection/connection.hpp"
#include "../../../view/view.hpp"

using tcp = boost::asio::ip::tcp;
using errorCode = boost::system::error_code;

class ClientMadnBoard {
    private:
        boost::asio::io_service io_service;
        int playerNumber;
        bool aktiv;
        ViewPtr view;
    public:
        ClientMadnBoard (ViewPtr v);
       // ClientMadn GetClient();
        void readHandler2 (connection_madn_ptr connection_ptr);
        void writeHandler (connection_madn_ptr connection_ptr);
        void readHandler1 (connection_madn_ptr connection_ptr);
        // void connectionHandler (connection_madn_ptr connection_ptr);
        void startClient (std::string ip, std::string port);
};

using ClientMadnBoardPtr = std::shared_ptr<ClientMadnBoard>;

#endif