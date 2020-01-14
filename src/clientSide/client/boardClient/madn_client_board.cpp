#include "madn_client_board.hpp"
#include <sstream>

ClientMadnBoard::ClientMadnBoard(ViewPtr v) {
    this->view = v;
}

void ClientMadnBoard::readHandler2 (connection_madn_ptr connection_ptr) {
    std::string answer(connection_ptr->connection_buffer);
    this->view->setPositions(answer);
    auto boostBuffer = boost::asio::buffer(connection_ptr->connection_buffer, 100);
            boost::asio::async_read(connection_ptr->socket, boostBuffer, [this, connection_ptr](boost::system::error_code errorCode1, size_t length){
                if (!errorCode1) this->readHandler1(connection_ptr);
            });
}

void ClientMadnBoard::writeHandler (connection_madn_ptr connection_ptr) {
    auto boostBuffer = boost::asio::buffer(connection_ptr->connection_buffer, 100);
    boost::asio::async_read(connection_ptr->socket, boostBuffer, [this, connection_ptr](errorCode errorCode, size_t length){
        if (!errorCode) this->readHandler2(connection_ptr);
    });
}

void ClientMadnBoard::readHandler1 (connection_madn_ptr connection_ptr) {
    std::this_thread::sleep_for(std::chrono::milliseconds(400));
    std:: string request("N");
    char buffer[100];
    strcpy(buffer, request.c_str());
    auto boostBuffer = boost::asio::buffer(buffer);
    boost::asio::async_write(connection_ptr->socket, boostBuffer, [this, connection_ptr](errorCode errorCode, size_t length){
        if (!errorCode) this->writeHandler(connection_ptr);
    });
}

void ClientMadnBoard::startClient (std::string ip, std::string port) {
    boost::asio::io_service io_service;
    connection_madn_ptr connection_ptr = std::make_shared<connection_madn>(io_service);


    tcp::resolver resolver{io_service};
    tcp::resolver::query query(ip, port);
    auto it = resolver.resolve(query);

   auto connectionHandler = [this, connection_ptr](const boost::system::error_code& errorCode, auto x){
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
    
}