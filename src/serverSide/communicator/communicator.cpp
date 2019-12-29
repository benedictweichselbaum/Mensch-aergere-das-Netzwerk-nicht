#include "communicator.hpp"

// Create ServerGameStateCommunicator with default values for a new game.
ServerGameCommunicator::ServerGameCommunicator () : players(new int8_t[4]) {
    state = std::make_shared<GameStartState>();
    game = std::make_shared<MenschAergereDichNichtGame>();
    for (int i = 0; i < 4; ++i) players[i] = 0;
}

// Create ServerGameStateCommunicator with given save game file
ServerGameCommunicator::ServerGameCommunicator (std::string saveGamePath) : players(new int8_t[4]) {
    state = std::make_shared<GameStartState>();
    game = std::make_shared<MenschAergereDichNichtGame>(saveGamePath);
    for (int i = 0; i < 4; ++i) players[i] = 0;
}

/*
Master Method for reacting to player input that is coming from the server.
Depending on the state of the machine certain answers and behavior is triggered.
The state changes depending on the answers from the state classes.
*/
std::string ServerGameCommunicator::reactToPlayerInput (std::string input) {
    // Asking current state for answer on the input. (State pattern)
    std::string answer = state->reactToPlayerInput(input, game);
    std::string statePrefix = answer.substr(0, 2);
    std::string realAnswer = answer.substr(3);

    // Regular expressions to examine the state change prefixes.
    std::regex playerJoin("[1-4]");
    std::regex stateChangeMeeple("[1-4]m");
    std::regex stateChangeNextPlayer("[1-4]n");

    if (statePrefix.compare("xx") == 0) {
        if (std::regex_match(realAnswer, playerJoin)) {
            int playerIndex = (std::stoi(realAnswer)) -1;
            if (players[playerIndex] != 0) return "numberAlreadyTaken";
            else {
                players[playerIndex] = 1;
            }
            int numberOfNaturalPlayers = 0;
            for (int pI = 0; pI < 4; ++ pI)
                if (players[pI] == 1) numberOfNaturalPlayers++;
            
            if (numberOfNaturalPlayers == 4) {
                state = std::make_shared<PlayingPlayerOneDiceState>();
                return game->getBoardAsString();
            }
            return "successfulJoin";
        } else return realAnswer;
    } else if (std::regex_match(statePrefix, stateChangeMeeple)) {
        int playerNumber = std::stoi(statePrefix.substr(0, 1));
        switch (playerNumber) {
            case 1:
                state = std::make_shared<PlayingPlayerOneMeepleState>();
                break;
            case 2:
                state = std::make_shared<PlayingPlayerTwoMeepleState>();
                break;
            case 3:
                state = std::make_shared<PlayingPlayerThreeMeepleState>();
                break;
            case 4:
                state = std::make_shared<PlayingPlayerFourMeepleState>();
                break;
        }
        return realAnswer;
    } else if (std::regex_match(statePrefix, stateChangeNextPlayer)) {
        int playerNumber = std::stoi(statePrefix.substr(0, 1));
        game->getBoard()[60] = playerNumber;
        switch (playerNumber) {
            case 1:
                if (players[playerNumber - 1] == 1)
                    state = std::make_shared<PlayingPlayerOneDiceState>();
                else
                    state = std::make_shared<PlayingComOneState>(); // If a non natural player is following, the COM player gets called.
                break;
            case 2:
                if (players[playerNumber - 1] == 1)
                    state = std::make_shared<PlayingPlayerTwoDiceState>();
                else
                    state = std::make_shared<PlayingComTwoState>();
                break;
            case 3:
                if (players[playerNumber - 1] == 1)
                    state = std::make_shared<PlayingPlayerThreeDiceState>();
                else
                    state = std::make_shared<PlayingComThreeState>();
                break;
            case 4:
                if (players[playerNumber - 1] == 1)
                    state = std::make_shared<PlayingPlayerFourDiceState>();
                else
                    state = std::make_shared<PlayingComFourState>();
                break;
        }
        return realAnswer;
    } else return realAnswer;
}

/*
Every answer has a prefix to state if a state has to be changed after the action was performed.
xx: No state change
[1-4]n: Next Player [1-4]
[1-4]m: Player [1-4] choose Meeple
*/
std::string GameStartState::reactToPlayerInput (std::string input, MadnGame_Ptr game) {
    std::regex joinExpression("[1-4](join)");
    if (std::regex_match(input, joinExpression)) {
        return "xx|" + input.substr(0, 1);
    } else {
        return "xx|inputDoesNotMatchState";
    }
}

std::string PlayingPlayerOneDiceState::reactToPlayerInput (std::string input, MadnGame_Ptr game) {
    if (input.compare("N") == 0) return "xx|" + game->getBoardAsString();
    else if (input.compare("1D") == 0) {
        game->rollTheDice();
        return "1m|" + game->getBoardAsString();
    } else return "xx|notYourTurn";
}

std::string PlayingPlayerTwoDiceState::reactToPlayerInput (std::string input, MadnGame_Ptr game) {
    if (input.compare("N") == 0) return "xx|" + game->getBoardAsString();
    else if (input.compare("2D") == 0) {
        game->rollTheDice();
        return "2m|" + game->getBoardAsString();
    } else return "xx|notYourTurn";
}

std::string PlayingPlayerThreeDiceState::reactToPlayerInput (std::string input, MadnGame_Ptr game) {
    if (input.compare("N") == 0) return "xx|" + game->getBoardAsString();
    else if (input.compare("3D") == 0) {
        game->rollTheDice();
        return "2m|" + game->getBoardAsString();
    } else return "xx|notYourTurn";
}

std::string PlayingPlayerFourDiceState::reactToPlayerInput (std::string input, MadnGame_Ptr game) {
    if (input.compare("N") == 0) return "xx|" + game->getBoardAsString();
    else if (input.compare("4D") == 0) {
        game->rollTheDice();
        return "2m|" + game->getBoardAsString();
    } else return "xx|notYourTurn";
}

std::string PlayingPlayerOneMeepleState::reactToPlayerInput (std::string input, MadnGame_Ptr game) {
    std::regex chooseMeeple("[1-4][0-4]");
    if (input.compare("N") == 0) return "xx|" + game->getBoardAsString();
    else if (std::regex_match(input, chooseMeeple)) {
        int playerNumber = std::stoi(input.substr(0, 1));
        if (playerNumber != 1) { // Right Playernumber for state
            return "xx|notYourTurn";
        } else {
            std::string moveAnswer = game->movePlayerByPlayerNumberAndDiceNumberAndMeepleNumber(playerNumber, std::stoi(input.substr(1)));
            if (moveAnswer.compare("success") == 0) {
                return "2n|" + game->getBoardAsString();
            } else if (moveAnswer.compare("win") == 0) {
                return "xx|" + game->getBoardAsString();
            } else if (moveAnswer.compare("again") == 0) {
                return "1n|" + game->getBoardAsString();
            } else {
                return "xx|moveNotPossible";
            }
        }
    } else return "xx|inputDoesNotMatchState";
}

std::string PlayingPlayerTwoMeepleState::reactToPlayerInput (std::string input, MadnGame_Ptr game) {
    std::regex chooseMeeple("[1-4][1-4]");
    if (input.compare("N") == 0) return "xx|" + game->getBoardAsString();
    else if (std::regex_match(input, chooseMeeple)) {
        int playerNumber = std::stoi(input.substr(0, 1));
        if (playerNumber != 2) { // Right Playernumber for state
            return "xx|notYourTurn";
        } else {
            std::string moveAnswer = game->movePlayerByPlayerNumberAndDiceNumberAndMeepleNumber(playerNumber, std::stoi(input.substr(1)));
            if (moveAnswer.compare("success") == 0) {
                return "3n|" + game->getBoardAsString();
            } else if (moveAnswer.compare("win") == 0) {
                return "xx|" + game->getBoardAsString();
            } else if (moveAnswer.compare("again") == 0) {
                return "2n|" + game->getBoardAsString();
            } else {
                return "xx|moveNotPossible";
            }
        }
    } else return "xx|inputDoesNotMatchState";
}

std::string PlayingPlayerThreeMeepleState::reactToPlayerInput (std::string input, MadnGame_Ptr game) {
    std::regex chooseMeeple("[1-4][1-4]");
    if (input.compare("N") == 0) return "xx|" + game->getBoardAsString();
    else if (std::regex_match(input, chooseMeeple)) {
        int playerNumber = std::stoi(input.substr(0, 1));
        if (playerNumber != 3) { // Right Playernumber for state
            return "xx|notYourTurn";
        } else {
            std::string moveAnswer = game->movePlayerByPlayerNumberAndDiceNumberAndMeepleNumber(playerNumber, std::stoi(input.substr(1)));
            if (moveAnswer.compare("success") == 0) {
                return "4n|" + game->getBoardAsString();
            } else if (moveAnswer.compare("win") == 0) {
                return "xx|" + game->getBoardAsString();
            } else if (moveAnswer.compare("again") == 0) {
                return "3n|" + game->getBoardAsString();
            } else {
                return "xx|moveNotPossible";
            }
        }
    } else return "xx|inputDoesNotMatchState";
}

std::string PlayingPlayerFourMeepleState::reactToPlayerInput (std::string input, MadnGame_Ptr game) {
    std::regex chooseMeeple("[1-4][1-4]");
    if (input.compare("N") == 0) return "xx|" + game->getBoardAsString();
    else if (std::regex_match(input, chooseMeeple)) {
        int playerNumber = std::stoi(input.substr(0, 1));
        if (playerNumber != 4) { // Right Playernumber for state
            return "xx|notYourTurn";
        } else {
            std::string moveAnswer = game->movePlayerByPlayerNumberAndDiceNumberAndMeepleNumber(playerNumber, std::stoi(input.substr(1)));
            if (moveAnswer.compare("success") == 0) {
                return "1n|" + game->getBoardAsString();
            } else if (moveAnswer.compare("win") == 0) {
                return "xx|" + game->getBoardAsString();
            } else if (moveAnswer.compare("again") == 0) {
                return "4n|" + game->getBoardAsString();
            } else {
                return "xx|moveNotPossible";
            }
        }
    } else return "xx|inputDoesNotMatchState";
}

std::string PlayingComOneState::reactToPlayerInput (std::string input, MadnGame_Ptr game) {
    if (input.compare("N") == 0) return "xx|" + game->getBoardAsString();
}

std::string PlayingComTwoState::reactToPlayerInput (std::string input, MadnGame_Ptr game) {
    if (input.compare("N") == 0) return "xx|" + game->getBoardAsString();
}

std::string PlayingComThreeState::reactToPlayerInput (std::string input, MadnGame_Ptr game) {
    if (input.compare("N") == 0) return "xx|" + game->getBoardAsString();
}

std::string PlayingComFourState::reactToPlayerInput (std::string input, MadnGame_Ptr game) {
    if (input.compare("N") == 0) return "xx|" + game->getBoardAsString();
}