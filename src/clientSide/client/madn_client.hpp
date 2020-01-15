#ifndef H_MADNCLIENT
#define H_MADNCLIENT

#include <SFML/Graphics.hpp>
#include <memory>
#include <iostream>
#include <SFML/System/Sleep.hpp>
#include "../../client_server_connection/connection.hpp"

#define ERRORANSWERFROMSERVER "refused" // Antwort wenn Spielernummer vergeben

using tcp = boost::asio::ip::tcp;
using errorCode = boost::system::error_code;

std::string GetCurrentDateWithTime_client ();

/*
 * Client for the normal player. Sends the game logic relevant messages to the server.
 */
class ClientMadn {
    private:
        boost::asio::io_service io_service;
    public:
        ClientMadn ();
        int playerNumber;
        std::string answer;
        std::string message;
		bool aktiv;
        int statusRunningHandler; //0: warten bis View setzt / 1: View hat message gesetzt -> Client sendet und empf�ngt / 2: Client hat empfangen und View kann answer holen / 0: View hat answer geholt // Sondefall 4: Client hat noch keine Verbindung aufgebaut
        void RunningReadHandler1(connection_madn_ptr connection_ptr);
        void RunningReadHandler2(connection_madn_ptr connection_ptr);
        void RunningWriteHandler(connection_madn_ptr connection_ptr);
        void ReadHandler2 (connection_madn_ptr connection_ptr);
        void WriteHandler (connection_madn_ptr connection_ptr);
        void ReadHandler1 (connection_madn_ptr connection_ptr);
        void StartClient (std::string ip, std::string port);
};

using ClientMadnPtr = std::shared_ptr<ClientMadn>;

#endif