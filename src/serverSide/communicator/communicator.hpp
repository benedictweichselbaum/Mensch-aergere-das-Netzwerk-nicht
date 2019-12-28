#ifndef H_COMMUNICATOR
#define H_COMMUNICATOR

#include "../../game/madn_game.hpp"
#include <memory>
#include <regex>


class GameCommunicatorState {
    public:
        virtual std::string reactToPlayerInput (std::string input, MadnGame_Ptr game) = 0;
};

class GameStartState : public GameCommunicatorState {
    public:
        virtual std::string reactToPlayerInput (std::string input, MadnGame_Ptr game) override;
};

class PlayingState : public GameCommunicatorState {
    public:
        virtual std::string reactToPlayerInput (std::string input, MadnGame_Ptr game) override;
};

using GameCommunicatorState_Ptr = std::shared_ptr<GameCommunicatorState>;

class ServerGameCommunicator {
    private:
        MadnGame_Ptr game;
        GameCommunicatorState_Ptr state;
        int8_t* players; // Index+1 matches playerNumber. 1 = Real Player; 0 = COM.
    public:
        ServerGameCommunicator();
        ServerGameCommunicator (std::string saveGamePath);
        ~ServerGameCommunicator () {
            delete[] players;
        };
        std::string reactToPlayerInput (std::string input);
        MadnGame_Ptr getGame () {return game;};
};

using ServerGameCommunicator_Ptr = std::shared_ptr<ServerGameCommunicator>;

#endif