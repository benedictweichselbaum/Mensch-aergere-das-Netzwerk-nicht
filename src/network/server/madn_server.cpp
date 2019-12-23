#include "madn_server.hpp"

#define MSG_SIZE 100

std::string getCurrentDate ();

ServerMadn::ServerMadn () : acceptor (io_service) {}

void ServerMadn::handlePlayerInput (connection_madn_ptr con_ptr) {
    std::cout << "Log[" << getCurrentDate() << "]: " << "Player sent the following message: "
        << con_ptr->connection_buffer
        << std::endl;
    
    std::string clientMessage (con_ptr->connection_buffer);
    std::string dummyAnswer1 = "This is the first answer!";
    std::string dummyAnswer2 = "This is the second answer!";
    std::string returnString = (clientMessage == "First") ? dummyAnswer1 : dummyAnswer2;

    char returnStringBuffer[MSG_SIZE];
    strcpy(returnStringBuffer, returnString.c_str());

    auto boostBuffer = boost::asio::buffer(returnStringBuffer, MSG_SIZE);
    boost::asio::async_write(con_ptr->socket, boostBuffer, [this, con_ptr](const boost::system::error_code& errorCode, size_t length){
        if (!errorCode) {
            std::cout << "Log[" << getCurrentDate() << "]: "
            << "Communication was successfull."
            << std::endl;
        }
    });
}

void ServerMadn::getClientPlayerInput (connection_madn_ptr con_ptr) {
    std::cout << "Log[" << getCurrentDate() << "]: " << "Reading new input." << std::endl;
    auto boostBuffer = boost::asio::buffer (con_ptr->connection_buffer, MSG_SIZE);
    boost::asio::async_read(con_ptr->socket, boostBuffer, [this, con_ptr](const boost::system::error_code& errorCode, size_t length){
        if (!errorCode) this->handlePlayerInput(con_ptr);
    });
}

void ServerMadn::welcomeNewClient (connection_madn_ptr con_ptr) {
    std::string greeting = "You succsessfully established a connection. Let's Play!\n";
    char welcomeMessage[MSG_SIZE];
    strcpy(welcomeMessage, greeting.c_str());

    auto boostBuffer = boost::asio::buffer(welcomeMessage, MSG_SIZE);
    boost::asio::async_write(con_ptr->socket, boostBuffer, [this, con_ptr](const boost::system::error_code& errorCode, size_t length){
        if (!errorCode) this->getClientPlayerInput(con_ptr);
    });
}

void ServerMadn::handleAcceptedConnection (connection_madn_ptr con_ptr) {
    std::cout << "Log[" << getCurrentDate() << "]: " << "New established connection got accepted." << std::endl;
    welcomeNewClient (con_ptr);
    // Create new connection in order to continue the event loop.
    auto newConPtr = std::make_shared<connection_madn>(io_service);
    acceptor.async_accept(newConPtr->socket, [this, newConPtr](const boost::system::error_code& errorCode){
        if (!errorCode) this->handleAcceptedConnection(newConPtr);
    });
}

void ServerMadn::startServer (ushort port) {
    auto endpoint = tcp::endpoint (tcp::v4(), port);
    
    std::cout << "Log[" << getCurrentDate() << "]: " << "Server started." << std::endl;

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

int main () {
    ServerMadn serverMadn;
    serverMadn.startServer(8999);
    return 0x00;
}