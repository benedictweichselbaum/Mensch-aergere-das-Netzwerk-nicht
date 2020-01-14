#include "madn_client.hpp"
#include <sstream>

ClientMadn::ClientMadn() {

}

std::string GetCurrentDateWithTime_client () {
    auto currentTime_t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    char stringDateBuffer[100] = {0};
    // tm timeinfo;
    // localtime_s(&timeinfo, &currentTime_t);
    // std::strftime(stringDateBuffer, sizeof(stringDateBuffer), "%Y-%m-%d %H:%M:%S", &timeinfo);
    std::strftime(stringDateBuffer, sizeof(stringDateBuffer), "%Y-%m-%d %H:%M:%S", std::localtime(&currentTime_t)); //Die drei Zeilen darüber können diese beiden Zeilen ersetzen und sind dann threadsafe (localtime_s hat aber bei Linux Probleme bereitet)
    return stringDateBuffer;
}

void ClientMadn::ReadHandler2 (connection_madn_ptr connection_ptr) {
    std::string answer(connection_ptr->connection_buffer);
    auto boostBuffer = boost::asio::buffer(connection_ptr->connection_buffer, 100);
    if (answer != ERRORANSWERFROMSERVER)
    {
        this->statusRunningHandler = 0;
        this->aktiv = true;
        boost::asio::async_read(connection_ptr->socket, boostBuffer, [this, connection_ptr](boost::system::error_code errorCode1, size_t length) {
            if (!errorCode1) this->RunningReadHandler1(connection_ptr);
            });
    }
    else 
    {
        ++this->playerNumber;
        boost::asio::async_read(connection_ptr->socket, boostBuffer, [this, connection_ptr](boost::system::error_code errorCode1, size_t length) {
            if (!errorCode1) this->ReadHandler1(connection_ptr);
            });
    }
   
}

void ClientMadn::WriteHandler (connection_madn_ptr connection_ptr) 
{
    auto boostBuffer = boost::asio::buffer(connection_ptr->connection_buffer, 100);
    boost::asio::async_read(connection_ptr->socket, boostBuffer, [this, connection_ptr](errorCode errorCode, size_t length){
        if (!errorCode) this->ReadHandler2(connection_ptr);
    });
}

void ClientMadn::ReadHandler1 (connection_madn_ptr connection_ptr) {
    if (this->playerNumber > 4)
    {
        this->playerNumber == 1;
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        std::cout << "Kein offenes Spiel gefunden, nächster Versuch in 0,5 Sekunden." << std::endl;
    }
    std::string request(std::to_string(this->playerNumber) + "join");
    char buffer[100];
    strcpy(buffer, request.c_str());
    auto boostBuffer = boost::asio::buffer(buffer);
    boost::asio::async_write(connection_ptr->socket, boostBuffer, [this, connection_ptr](errorCode errorCode, size_t length) {
        if (!errorCode) this->WriteHandler(connection_ptr);
        });
}

void ClientMadn::StartClient (std::string ip, std::string port) {
    this->playerNumber = 1;
    this->statusRunningHandler = 4;
    boost::asio::io_service io_service;
    connection_madn_ptr connection_ptr = std::make_shared<connection_madn>(io_service);

    tcp::resolver resolver{ io_service };
    tcp::resolver::query query(ip, port);
    auto it = resolver.resolve(query);

    auto connectionHandler = [this, connection_ptr](const boost::system::error_code& errorCode, auto x) {
        if (!errorCode) {
            auto boostBuffer = boost::asio::buffer(connection_ptr->connection_buffer, 100);
            boost::asio::async_read(connection_ptr->socket, boostBuffer, [this, connection_ptr](boost::system::error_code errorCode1, size_t length) {
                if (!errorCode1) this->ReadHandler1(connection_ptr);
                });
        }
    };
    boost::asio::async_connect(connection_ptr->socket, it, connectionHandler);
    auto t = std::thread([&io_service]() {io_service.run(); });
    t.join();
}

void ClientMadn::RunningReadHandler2(connection_madn_ptr connection_ptr) {
    std::string answer(connection_ptr->connection_buffer);
    this->answer = answer;
    this->statusRunningHandler = 2;
    auto boostBuffer = boost::asio::buffer(connection_ptr->connection_buffer, 100);
    if(aktiv)
    {
        boost::asio::async_read(connection_ptr->socket, boostBuffer, [this, connection_ptr](boost::system::error_code errorCode1, size_t length) {
            if (!errorCode1) this->RunningReadHandler1(connection_ptr);
            });
    }
}

void ClientMadn::RunningWriteHandler(connection_madn_ptr connection_ptr) {
    auto boostBuffer = boost::asio::buffer(connection_ptr->connection_buffer, 100);
    boost::asio::async_read(connection_ptr->socket, boostBuffer, [this, connection_ptr](errorCode errorCode, size_t length) {
        if (!errorCode) this->RunningReadHandler2(connection_ptr);
        });
}

void ClientMadn::RunningReadHandler1(connection_madn_ptr connection_ptr) {
    while (this->statusRunningHandler != 1)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
    char buffer[100];
    strcpy(buffer, this->message.c_str());
    auto boostBuffer = boost::asio::buffer(buffer);
    boost::asio::async_write(connection_ptr->socket, boostBuffer, [this, connection_ptr](errorCode errorCode, size_t length) {
        if (!errorCode) this->RunningWriteHandler(connection_ptr);
        });
}

std::string IntToString(int number)
{
    std::stringstream s;
    s << number;
    return s.str();
}