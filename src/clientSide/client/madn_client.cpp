#include "madn_client.hpp"
#include <sstream>

ClientMadn::ClientMadn() {

}

//ClientMadn ClientMadn::GetClient()
//{
//    return *this;
//}

std::string getCurrentDateWithTime_client () {
    auto currentTime_t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    char stringDateBuffer[100] = {0};
    // tm timeinfo;
    // localtime_s(&timeinfo, &currentTime_t);
    // std::strftime(stringDateBuffer, sizeof(stringDateBuffer), "%Y-%m-%d %H:%M:%S", &timeinfo);
    std::strftime(stringDateBuffer, sizeof(stringDateBuffer), "%Y-%m-%d %H:%M:%S", std::localtime(&currentTime_t)); //Die beiden Zeilen darüber ersetzen diese Funktion auch threadsafe
    return stringDateBuffer;
}

void ClientMadn::readHandler2 (connection_madn_ptr connection_ptr) {
    std::string answer(connection_ptr->connection_buffer);
    auto boostBuffer = boost::asio::buffer(connection_ptr->connection_buffer, 100);
    if (answer != ERRORANSWERFROMSERVER)
    {
        this->statusRunningHandler = 0;
        boost::asio::async_read(connection_ptr->socket, boostBuffer, [this, connection_ptr](boost::system::error_code errorCode1, size_t length) {
            if (!errorCode1) this->RunningReadHandler1(connection_ptr);
            });
    }
    else 
    {
        ++this->playerNumber;
        boost::asio::async_read(connection_ptr->socket, boostBuffer, [this, connection_ptr](boost::system::error_code errorCode1, size_t length) {
            if (!errorCode1) this->readHandler1(connection_ptr);
            });
    }
   
}

void ClientMadn::writeHandler (connection_madn_ptr connection_ptr) {
    auto boostBuffer = boost::asio::buffer(connection_ptr->connection_buffer, 100);
    boost::asio::async_read(connection_ptr->socket, boostBuffer, [this, connection_ptr](errorCode errorCode, size_t length){
        if (!errorCode) this->readHandler2(connection_ptr);
    });
}

void ClientMadn::readHandler1 (connection_madn_ptr connection_ptr) {
    if (this->playerNumber > 4)
    {
        this->playerNumber == 1;
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        std::cout << "Kein offenes Spiel gefunden, nächster Versuch in 0,5 Sekunden." << std::endl;
    }
    std::string request(IntToString(this->playerNumber) + "join");
    char buffer[100];
    strcpy(buffer, request.c_str());
    auto boostBuffer = boost::asio::buffer(buffer);
    boost::asio::async_write(connection_ptr->socket, boostBuffer, [this, connection_ptr](errorCode errorCode, size_t length) {
        if (!errorCode) this->writeHandler(connection_ptr);
        });
}

void ClientMadn::startClient (std::string ip, std::string port) {
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
                if (!errorCode1) this->readHandler1(connection_ptr);
                });
        }
    };
    boost::asio::async_connect(connection_ptr->socket, it, connectionHandler);
    auto t = std::thread([&io_service]() {io_service.run(); });
    t.join();
}

void ClientMadn::send(std::string message)
{
    std::cout << "Log[" << getCurrentDateWithTime_client() << "]: " << "Send:" << message << std::endl;
    char buffer[100];
    strcpy(buffer, message.c_str());
    auto boostBuffer = boost::asio::buffer(buffer);
    //auto boostBuffer = boost::asio::buffer(connection_ptr->connection_buffer, 100);
    /*boost::asio::async_read(connection_ptr->socket, boostBuffer, [this, connection_ptr](errorCode errorCode, size_t length) {
        if (!errorCode) this->readHandler2(connection_ptr);
        });*/
    
}

int ClientMadn::StipulatePlayerNumber()
{
    std::string answer = "";
    int number = 0;
    while (answer != ERRORANSWERFROMSERVER && number <= 4)
    {
        ++number;
        send(number + "join");
        answer = AwaitServerResponse();
    }
    if (answer == ERRORANSWERFROMSERVER)
    {
        std::cout << "Konnte keine Verbindung aufbauen, versuche es in 5 Sekunden erneut."; 
        sf::sleep(sf::seconds(5));
        return StipulatePlayerNumber();
    }
    else
    {
        return number;
    }
}

std::string ClientMadn::AwaitServerResponse()
{
    return std::string();
}


void ClientMadn::MessageHandler()
{
    while (aktiv)
    {
        //TODO
        // Nachricht an IncommingMessage Übergeben
    }
}

void ClientMadn::RunGame()
{
    aktiv = true;
    MessageHandler(); //TODO in Thread auslagern
}

void ClientMadn::SendStart()
{
    send(playerNumber + "start");
}

std::string ClientMadn::Send(std::string message)
{
    //TODO: An Server senden und Antwort zurückgeben
}

void ClientMadn::SendMeepleClicked(int player, int meeple)
{
    if (player == playerNumber)
    {
        send(playerNumber + "" + meeple);
    } //TODO evtl. Fehlermeldung wenn falsche angeklickt
}

void ClientMadn::SendRollTheDice()
{
    send(playerNumber + "D");
}

void ClientMadn::SendPass()
{
    send(playerNumber + "P");
}

void ClientMadn::SendQuit()
{
    send(playerNumber + "quit");
}

/* int main () {
    ClientMadnPtr client = std::make_shared<ClientMadn>();
    client->startClient("localhost", "8999");
    sf::RenderWindow window(sf::VideoMode(PLAYGROUNDINTERVAL * 11 + SIDEPANEL, PLAYGROUNDINTERVAL * 11), "Mensch ärgere dich nicht");
    window.setFramerateLimit(30);

    client->RunGame();

    std::cout << "Log[" << getCurrentDateWithTime_client() << "]: " << "Client stopped." << std::endl;
    return 0x00;
}*/

//TODO: Wenn Spiel beendet (unterbrochen) ohne Sieger, aktiv auf false setzen


void ClientMadn::RunningReadHandler2(connection_madn_ptr connection_ptr) {
    std::string answer(connection_ptr->connection_buffer);
    this->answer = answer;
    this->statusRunningHandler = 2;
    auto boostBuffer = boost::asio::buffer(connection_ptr->connection_buffer, 100);
    boost::asio::async_read(connection_ptr->socket, boostBuffer, [this, connection_ptr](boost::system::error_code errorCode1, size_t length) {
        if (!errorCode1) this->RunningReadHandler1(connection_ptr);
        });
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