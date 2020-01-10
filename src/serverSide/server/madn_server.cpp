#include "madn_server.hpp"

#define MSG_SIZE 100

ServerMadn::ServerMadn () : acceptor (io_service) {
    serverGameCommunicator = std::make_shared<ServerGameCommunicator>();
}

ServerMadn::ServerMadn (std::string saveGameFile) : acceptor (io_service) {
    serverGameCommunicator = std::make_shared<ServerGameCommunicator>(saveGameFile);
} 

void ServerMadn::handlePlayerInput (connection_madn_ptr con_ptr) {
    std::cout << "Log[" << getCurrentDateWithTime() << "]: " << "Player sent the following message: "
        << con_ptr->connection_buffer
        << std::endl;
    
    std::string clientMessage (con_ptr->connection_buffer);
    std::string returnString = serverGameCommunicator->reactToPlayerInput(clientMessage);
    char returnStringBuffer[MSG_SIZE];
    strcpy_s(returnStringBuffer, returnString.c_str());

    auto boostBuffer = boost::asio::buffer(returnStringBuffer, MSG_SIZE);
    boost::asio::async_write(con_ptr->socket, boostBuffer, [this, con_ptr](const boost::system::error_code& errorCode, size_t length){
        if (!errorCode) {
            std::cout << "Log[" << getCurrentDateWithTime() << "]: "
            << "Communication was successfull."
            << std::endl;
            this->askForNewInput(con_ptr);
        }
    });
}

void ServerMadn::getClientPlayerInput (connection_madn_ptr con_ptr) {
    std::cout << "Log[" << getCurrentDateWithTime() << "]: " << "Reading new input." << std::endl;
    auto boostBuffer = boost::asio::buffer (con_ptr->connection_buffer, MSG_SIZE);
    boost::asio::async_read(con_ptr->socket, boostBuffer, [this, con_ptr](const boost::system::error_code& errorCode, size_t length){
        if (!errorCode) this->handlePlayerInput(con_ptr);
    });
}

void ServerMadn::askForNewInput (connection_madn_ptr con_ptr) {
    std::string greeting = "Enter new input:";
    char welcomeMessage[MSG_SIZE];
    strcpy_s(welcomeMessage, greeting.c_str());

    auto boostBuffer = boost::asio::buffer(welcomeMessage, MSG_SIZE);
    boost::asio::async_write(con_ptr->socket, boostBuffer, [this, con_ptr](const boost::system::error_code& errorCode, size_t length){
        if (!errorCode) this->getClientPlayerInput(con_ptr);
    });
}

void ServerMadn::handleAcceptedConnection (connection_madn_ptr con_ptr) {
    std::cout << "Log[" << getCurrentDateWithTime() << "]: " << "New established connection got accepted." << std::endl;
    askForNewInput (con_ptr);

    // Create new connection in order to continue the event loop.
    auto newConPtr = std::make_shared<connection_madn>(io_service);
    acceptor.async_accept(newConPtr->socket, [this, newConPtr](const boost::system::error_code& errorCode){
        if (!errorCode) this->handleAcceptedConnection(newConPtr);
    });
}

void ServerMadn::startServer (unsigned short port) {
    auto endpoint = tcp::endpoint (tcp::v4(), port);
    
    std::cout << "Log[" << getCurrentDateWithTime() << "]: " << "Server started on port " << port << "." << std::endl;

    acceptor.open(endpoint.protocol());
    acceptor.bind(endpoint);
    acceptor.listen();

    auto connection_ptr = std::make_shared<connection_madn>(io_service);
    acceptor.async_accept(connection_ptr->socket, [this, connection_ptr](const boost::system::error_code& errorCode){
        if (!errorCode) this->handleAcceptedConnection (connection_ptr);
    });

    auto eventLoopFunction = [this](){
        io_service.run();
    };
    thread_ptr = std::make_unique<std::thread>(eventLoopFunction);
    thread_ptr->join();
}

std::string getCurrentDateWithTime () {
    auto currentTime_t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    char stringDateBuffer[100] = {0};
    tm timeinfo;
    localtime_s(&timeinfo, &currentTime_t);
    std::strftime(stringDateBuffer, sizeof(stringDateBuffer), "%Y-%m-%d %H:%M:%S", &timeinfo);
   // std::strftime(stringDateBuffer, sizeof(stringDateBuffer), "%Y-%m-%d %H:%M:%S", std::localtime(&currentTime_t)); //Die beiden Zeilen darüber ersetzen diese Funktion auch threadsafe
    return stringDateBuffer;
}