#include "communicator.hpp"

ServerGameCommunicator::ServerGameCommunicator () : players(new int8_t[4]) {
    state = std::make_shared<GameStartState>();
    game = std::make_shared<MenschAergereDichNichtGame>();
}

ServerGameCommunicator::ServerGameCommunicator (std::string saveGamePath) : players(new int8_t[4]) {
    state = std::make_shared<GameStartState>();
    game = std::make_shared<MenschAergereDichNichtGame>(saveGamePath);
}

std::string ServerGameCommunicator::reactToPlayerInput (std::string input) {
    std::string answer = state->reactToPlayerInput(input, game);
    std::regex playerJoin("[1-4]");
    if (std::regex_match(answer, playerJoin)) {
        int playerIndex = ((int) answer.at(0)) - 1;
        if (players[playerIndex] == 1) {
            return "numberAlreadyTaken";
        } else {
            players[playerIndex] = 1;
            return "successfulJoin";    
        }
    } else {
        return answer;
    }
}

std::string GameStartState::reactToPlayerInput (std::string input, MadnGame_Ptr game) {
    std::regex joinExpression("[1-4](join)");
    if (std::regex_match(input, joinExpression)) {
        std::string returnString = "";
        returnString += input.at(0);
        return returnString;
    } else {
        return "inputDoesNotMatchState";
    }
}

std::string PlayingState::reactToPlayerInput (std::string input, MadnGame_Ptr game) {
    return game->getBoardAsString();
}