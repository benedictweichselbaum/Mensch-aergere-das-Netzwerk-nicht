#ifndef H_COMMUNICATOR
#define H_COMMUNICATOR

#include "../../game/madn_game.hpp"
#include <memory>
#include <regex>
#include <string>

/*
The following header declares a class that acts as a communicator between the server that is
recieving text massages from the clients and the actual game logic. To determine which players
turn it is and weather or not COMs are involed (and many more things :D) the communicator has a
state which is implemented via a 'State Pattern'. All states extend an 'abstract' super class 
with an 'abstract' method called'reactToPlayerInput'. Depending on the current state and its own
definition of the method different actions are triggered.
*/


// Abstract state super class
class GameCommunicatorState {
    public:
        virtual std::string reactToPlayerInput (std::string input, MadnGame_Ptr game) = 0;
};

class GameStartState : public GameCommunicatorState {
    public:
        virtual std::string reactToPlayerInput (std::string input, MadnGame_Ptr game) override;
};

class PlayingPlayerOneDiceState : public GameCommunicatorState {
    public:
        virtual std::string reactToPlayerInput (std::string input, MadnGame_Ptr game) override;
};

class PlayingPlayerTwoDiceState : public GameCommunicatorState {
    public:
        virtual std::string reactToPlayerInput (std::string input, MadnGame_Ptr game) override;
};

class PlayingPlayerThreeDiceState : public GameCommunicatorState {
    public:
        virtual std::string reactToPlayerInput (std::string input, MadnGame_Ptr game) override;
};

class PlayingPlayerFourDiceState : public GameCommunicatorState {
    public:
        virtual std::string reactToPlayerInput (std::string input, MadnGame_Ptr game) override;
};

class PlayingPlayerOneMeepleState : public GameCommunicatorState {
    public:
        virtual std::string reactToPlayerInput (std::string input, MadnGame_Ptr game) override;
};

class PlayingPlayerTwoMeepleState : public GameCommunicatorState {
    public:
        virtual std::string reactToPlayerInput (std::string input, MadnGame_Ptr game) override;
};

class PlayingPlayerThreeMeepleState : public GameCommunicatorState {
    public:
        virtual std::string reactToPlayerInput (std::string input, MadnGame_Ptr game) override;
};

class PlayingPlayerFourMeepleState : public GameCommunicatorState {
    public:
        virtual std::string reactToPlayerInput (std::string input, MadnGame_Ptr game) override;
};

class PlayingComOneState : public GameCommunicatorState {
    public:
        virtual std::string reactToPlayerInput (std::string input, MadnGame_Ptr game) override;
};

class PlayingComTwoState : public GameCommunicatorState {
    public:
        virtual std::string reactToPlayerInput (std::string input, MadnGame_Ptr game) override;
};

class PlayingComThreeState : public GameCommunicatorState {
    public:
        virtual std::string reactToPlayerInput (std::string input, MadnGame_Ptr game) override;
};

class PlayingComFourState : public GameCommunicatorState {
    public:
        virtual std::string reactToPlayerInput (std::string input, MadnGame_Ptr game) override;
};

class PlayingPlayerOneDiceOutOfHouseState : public GameCommunicatorState {
    public:
        virtual std::string reactToPlayerInput (std::string input, MadnGame_Ptr game) override;
};

class PlayingPlayerTwoDiceOutOfHouseState : public GameCommunicatorState {
    public:
        virtual std::string reactToPlayerInput (std::string input, MadnGame_Ptr game) override;
};

class PlayingPlayerThreeDiceOutOfHouseState : public GameCommunicatorState {
    public:
        virtual std::string reactToPlayerInput (std::string input, MadnGame_Ptr game) override;
};

class PlayingPlayerFourDiceOutOfHouseState : public GameCommunicatorState {
    public:
        virtual std::string reactToPlayerInput (std::string input, MadnGame_Ptr game) override;
};

using GameCommunicatorState_Ptr = std::shared_ptr<GameCommunicatorState>;

class ServerGameCommunicator {
    private:
        MadnGame_Ptr game;
        GameCommunicatorState_Ptr state;
        int8_t* players; // Index+1 matches playerNumber. 1 = Real player; 0 = COM. Also used to count the times a player rolled the dice.
    public:
        ServerGameCommunicator();
        ServerGameCommunicator (std::string saveGamePath);
        ~ServerGameCommunicator () {
            delete[] players;
        };
        std::string reactToPlayerInput (std::string input);
        MadnGame_Ptr getGame () {return game;};
        bool playerStartingGame (int8_t playerNumber);
        void changeStateToNextPlayer (int8_t playerNumber);
};

using ServerGameCommunicator_Ptr = std::shared_ptr<ServerGameCommunicator>;

#endif