#ifndef H_SERVERMADN
#define H_SERVERMADN

#include "../../client_server_connection/connection.hpp"
#include "../communicator/communicator.hpp"


using tcp = boost::asio::ip::tcp;

/* MADN stands for 'Mensch aergere dich nicht!'
 * Server Class. Gets messages from client and gives answer.
*/

class ServerMadn {
    private:
        boost::asio::io_service io_service;
        tcp::acceptor acceptor;
        std::unique_ptr<std::thread> thread_ptr;
        ServerGameCommunicator_Ptr serverGameCommunicator;
    public:
        ServerMadn ();
        ServerMadn (std::string saveGameFile);
        void handlePlayerInput (connection_madn_ptr con_ptr);
        void getClientPlayerInput (connection_madn_ptr con_ptr);
        void askForNewInput (connection_madn_ptr con_ptr);
        void handleAcceptedConnection (connection_madn_ptr con_ptr);
        void startServer (ushort port);
};

std::string getCurrentDateWithTime ();

using ServerPtr = std::unique_ptr<ServerMadn>;

#endif