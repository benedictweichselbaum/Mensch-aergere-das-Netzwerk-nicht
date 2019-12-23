#pragma once

#include <boost/asio.hpp>
#include <iostream>
#include <memory>
#include <thread>
#include <chrono>

using tcp = boost::asio::ip::tcp;

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

std::string getCurrentDate () {
    auto currentTime_t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    char stringDateBuffer[100] = {0};
    std::strftime(stringDateBuffer, sizeof(stringDateBuffer), "%Y-%m-%d %H:%M:%S", std::localtime(&currentTime_t));
    return stringDateBuffer;
}