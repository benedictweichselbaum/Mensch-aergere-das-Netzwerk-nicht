#pragma once

#include "../../client_server_connection/connection.hpp"

using tcp = boost::asio::ip::tcp;
using errorCode = boost::system::error_code;

std::string getCurrentDateWithTime_client ();

class ClientMadn {
    private:
        boost::asio::io_service io_service;
    public:
        ClientMadn ();
        void readHandler2 (connection_madn_ptr connection_ptr);
        void writeHandler (connection_madn_ptr connection_ptr);
        void readHandler1 (connection_madn_ptr connection_ptr);
        // void connectionHandler (connection_madn_ptr connection_ptr);
        void startClient (std::string ip, std::string port);
};