#include "communicator.hpp"

ServerGameCommunicator::ServerGameCommunicator () : players(new int8_t[4]) {
    for (int index = 0; index < 4; ++index)
        players[index] = 0;
    state = std::make_shared<GameStartState>();
    game = std::make_shared<MenschAergereDichNichtGame>();
}

ServerGameCommunicator::ServerGameCommunicator (std::string saveGamePath) : players(new int8_t[4]) {
    for (int index = 0; index < 4; ++index)
        players[index] = 0;
    state = std::make_shared<GameStartState>();
    game = std::make_shared<MenschAergereDichNichtGame>(saveGamePath);
}

std::string ServerGameCommunicator::reactToPlayerInput (std::string input) {
    std::string answer = state->reactToPlayerInput(input, this->getGame());
    std::regex playerJoin("[1-4]");
    if (std::regex_match(answer, playerJoin)) {
        int playerIndex = (std::stoi(answer.c_str())) - 1;
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

std::string GameStartState::reactToPlayerInput (const std::string& input, MadnGame_Ptr game) {
    std::cout << input << std::endl;
    std::regex joinExpression("[1-4](join)");
    if (std::regex_match(input, joinExpression)) {
        return input.substr(0, 1);
    }
    return "inputDoesNotMatchState";
}

std::string PlayingState::reactToPlayerInput (const std::string& input, MadnGame_Ptr game) {
    return game->getBoardAsString();
}