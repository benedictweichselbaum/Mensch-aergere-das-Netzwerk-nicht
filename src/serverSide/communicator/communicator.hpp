#ifndef H_COMMUNICATOR
#define H_COMMUNICATOR

#include "../../game/madn_game.hpp"
#include <memory>
#include <regex>
#include <string>
#include <chrono>

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
        int8_t* players;
        /* Multipurpose array to execute certain game and application logic. Playernumber - 1 = Index in Array 
         * 1. 0 = COM; 1 = real Player
         * 2. Roll dice up to 3 times if all your meeples are in your starting house.
         * 3. Force starting a game when not all slots are filled with real players.
         * 4. Force quit a game if all real players want that.
         */
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
        void resetPlayersArrayFromQuitting ();
        void checkQuitting ();
};

using ServerGameCommunicator_Ptr = std::shared_ptr<ServerGameCommunicator>;

#endif