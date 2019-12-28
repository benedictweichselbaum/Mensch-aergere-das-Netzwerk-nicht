#include "madn_client.hpp"

ClientMadn::ClientMadn() {
}

std::string getCurrentDateWithTime () {
    auto currentTime_t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    char stringDateBuffer[100] = {0};
    std::strftime(stringDateBuffer, sizeof(stringDateBuffer), "%Y-%m-%d %H:%M:%S", std::localtime(&currentTime_t));
    return stringDateBuffer;
}

void ClientMadn::readHandler2 (connection_madn_ptr connection_ptr) {
    std::cout << "Log[" << getCurrentDateWithTime() << "]: " << "Server answer: " << connection_ptr->connection_buffer << std::endl;
    auto boostBuffer = boost::asio::buffer(connection_ptr->connection_buffer, 100);
            boost::asio::async_read(connection_ptr->socket, boostBuffer, [this, connection_ptr](boost::system::error_code errorCode1, size_t length){
                if (!errorCode1) this->readHandler1(connection_ptr);
            });
}

void ClientMadn::writeHandler (connection_madn_ptr connection_ptr) {
    std::cout << "Log[" << getCurrentDateWithTime() << "]: " << "Wait for reaction!" << std::endl;
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
    strcpy(buffer, answer.c_str());
    auto boostBuffer = boost::asio::buffer(buffer);
    boost::asio::async_write(connection_ptr->socket, boostBuffer, [this, connection_ptr](errorCode errorCode, size_t length){
        if (!errorCode) this->writeHandler(connection_ptr);
    });
}

void ClientMadn::startClient (std::string ip, std::string port) {
    std::cout << "Log[" << getCurrentDateWithTime() << "]: " << "Client started." << std::endl;
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
    std::cout << "Log[" << getCurrentDateWithTime() << "]: " << "Client stopped." << std::endl;
}

int main () {
    ClientMadn client;
    client.startClient("localhost", "8999");
    return 0x00;
}