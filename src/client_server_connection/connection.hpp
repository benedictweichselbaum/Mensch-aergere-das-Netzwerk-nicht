#ifndef H_CONNECTION
#define H_CONNECTION

#include <boost/asio.hpp>
#include <iostream>
#include <memory>
#include <thread>
#include <chrono>

using tcp = boost::asio::ip::tcp;

/*
 * Struct for the server client connection.
 */
typedef struct client_server_connection_madn {
    char* connection_buffer;
    tcp::socket socket;

    client_server_connection_madn (boost::asio::io_service& io_service) : socket(io_service), connection_buffer(new char[100]) {}
    ~client_server_connection_madn () {
        delete[] connection_buffer;
        socket.close();
    }
} connection_madn;

using connection_madn_ptr = std::shared_ptr<connection_madn>;

#endif