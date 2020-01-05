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
    std::regex stateChangeOutOfHouse("[1-4]h");
    std::regex playerWantsToStartExpression("[1-4]s");
    std::regex playerWantsToQuitExpression("[1-4]q");

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
                game->getBoard()[60] = 1;
                changeStateToNextPlayer(game->getBoard()[60]);
                return game->getBoardAsString();
            }
            return "successfulJoin";
        } else if (std::regex_match(realAnswer, playerWantsToStartExpression)) {
            if (playerStartingGame(std::stoi(realAnswer.substr(0, 1)))) return game->getBoardAsString();
            else return "sentSuccessfulStartingOrder";
        } else if (std::regex_match(realAnswer, playerWantsToQuitExpression)) {
            players[std::stoi(realAnswer.substr(0, 1)) - 1] = 9;
            checkQuitting();
            return "successfulQuitingOrder";
        } else return realAnswer;
    } else if (std::regex_match(statePrefix, stateChangeMeeple)) {
        resetPlayersArrayFromQuitting();
        int playerNumber = std::stoi(statePrefix.substr(0, 1));
        switch (playerNumber) {
            case 1:
                if (game->getBoard()[(playerNumber - 1) * 5] == 4 && players[playerNumber - 1] < 3) { // All Meeples are in house --> Dice up to 3x.
                    if (game->getCurrentDiceNumber() == 6) {
                        players[playerNumber - 1] = 1;
                        state = std::make_shared<PlayingPlayerOneMeepleState>();
                    } else {
                        players[playerNumber - 1]++;
                        // State does not change.
                    }
                } else if (players[playerNumber - 1] == 3) {
                    players[playerNumber - 1] = 1;
                    if (game->getCurrentDiceNumber() == 6) {
                        state = std::make_shared<PlayingPlayerOneMeepleState>();
                    } else {
                        if (players[playerNumber] == 0)
                            state = std::make_shared<PlayingComTwoState>();
                        else {
                            game->getBoard()[60] = 2;
                            state = std::make_shared<PlayingPlayerTwoDiceState>();
                            return game->getBoardAsString();
                        }
                    }
                } else 
                    state = std::make_shared<PlayingPlayerOneMeepleState>();
                break;
            case 2:
                if (game->getBoard()[(playerNumber - 1) * 5] == 4 && players[playerNumber - 1] < 3) { // All Meeples are in house --> Dice up to 3x.
                    if (game->getCurrentDiceNumber() == 6) {
                        players[playerNumber - 1] = 1;
                        state = std::make_shared<PlayingPlayerTwoMeepleState>();
                    } else {
                        players[playerNumber - 1]++;
                        // State does not change.
                    }
                } else if (players[playerNumber - 1] == 3) {
                    players[playerNumber - 1] = 1;
                    if (game->getCurrentDiceNumber() == 6) {
                        state = std::make_shared<PlayingPlayerTwoMeepleState>();
                    } else {
                        if (players[playerNumber] == 0)
                            state = std::make_shared<PlayingComThreeState>();
                        else {
                            game->getBoard()[60] = 3;
                            state = std::make_shared<PlayingPlayerThreeDiceState>();
                            return game->getBoardAsString();
                        }
                    }
                } else 
                    state = std::make_shared<PlayingPlayerTwoMeepleState>();
                break;
            case 3:
                if (game->getBoard()[(playerNumber - 1) * 5] == 4 && players[playerNumber - 1] < 3) { // All Meeples are in house --> Dice up to 3x.
                    if (game->getCurrentDiceNumber() == 6) {
                        players[playerNumber - 1] = 1;
                        state = std::make_shared<PlayingPlayerThreeMeepleState>();
                    } else {
                        players[playerNumber - 1]++;
                        // State does not change.
                    }
                } else if (players[playerNumber - 1] == 3) {
                    players[playerNumber - 1] = 1;
                    if (game->getCurrentDiceNumber() == 6) {
                        state = std::make_shared<PlayingPlayerThreeMeepleState>();
                    } else {
                        if (players[playerNumber] == 0)
                            state = std::make_shared<PlayingComFourState>();
                        else {
                            game->getBoard()[60] = 4;
                            state = std::make_shared<PlayingPlayerFourDiceState>();
                            return game->getBoardAsString();
                        }
                    }
                } else 
                    state = std::make_shared<PlayingPlayerThreeMeepleState>();
                break;
            case 4:
                if (game->getBoard()[(playerNumber - 1) * 5] == 4 && players[playerNumber - 1] < 3) { // All Meeples are in house --> Dice up to 3x.
                    if (game->getCurrentDiceNumber() == 6) {
                        players[playerNumber - 1] = 1;
                        state = std::make_shared<PlayingPlayerFourMeepleState>();
                    } else {
                        players[playerNumber - 1]++;
                        // State does not change.
                    }
                } else if (players[playerNumber - 1] == 3) {
                    players[playerNumber - 1] = 1;
                    if (game->getCurrentDiceNumber() == 6) {
                        state = std::make_shared<PlayingPlayerFourMeepleState>();
                    } else {
                        if (players[0] == 0)
                            state = std::make_shared<PlayingComOneState>();
                        else {
                            game->getBoard()[60] = 1;
                            state = std::make_shared<PlayingPlayerOneDiceState>();
                            return game->getBoardAsString();
                        }
                    }
                } else 
                    state = std::make_shared<PlayingPlayerFourMeepleState>();
                break;

            default: return realAnswer;
        }
        return realAnswer;
    } else if (std::regex_match(statePrefix, stateChangeNextPlayer)) {
        int playerNumber = std::stoi(statePrefix.substr(0, 1));
        game->getBoard()[60] = playerNumber;
        changeStateToNextPlayer(playerNumber);
        return game->getBoardAsString();
    } else if (std::regex_match(statePrefix, stateChangeOutOfHouse)) {
        resetPlayersArrayFromQuitting ();
        int playerNumber = std::stoi(statePrefix.substr(0, 1));
        switch (playerNumber) {
            case 1:
                state = std::make_shared<PlayingPlayerOneDiceOutOfHouseState>();
                break;
            case 2:
                state = std::make_shared<PlayingPlayerTwoDiceOutOfHouseState>();      
                break;          
            case 3:
                state = std::make_shared<PlayingPlayerThreeDiceOutOfHouseState>();
                break;
            case 4:
                 state = std::make_shared<PlayingPlayerFourDiceOutOfHouseState>();   
                 break;            
        }
        return realAnswer;
    } else return realAnswer;
}

bool ServerGameCommunicator::playerStartingGame (int8_t playerNumber) {
    players[playerNumber - 1] = 5; // 5 = S -> Start game.
    bool allPlayersWantToStart = true;
    for (int playerIndex = 0; playerIndex < 4; ++playerIndex) {
        if (players[playerIndex] == 1) allPlayersWantToStart = false;
    }
    if (allPlayersWantToStart) {
        for (int playerIndex = 0; playerIndex < 4; ++playerIndex) {
            if (players[playerIndex] != 0) players[playerIndex] = 1;
        }
        changeStateToNextPlayer(game->getBoard()[60]);
    }
    return allPlayersWantToStart;
}

void ServerGameCommunicator::changeStateToNextPlayer (int8_t playerNumber) {
    resetPlayersArrayFromQuitting();
    switch (playerNumber) {
        case 1:
            if (players[playerNumber - 1] == 0) {
                state = std::make_shared<PlayingComOneState>();
            } else {
                state = std::make_shared<PlayingPlayerOneDiceState>();
            }
            break;
        case 2:
            if (players[playerNumber - 1] == 0) {
                state = std::make_shared<PlayingComTwoState>();
            } else {
                state = std::make_shared<PlayingPlayerTwoDiceState>();
            }
            break;
        case 3:
            if (players[playerNumber - 1] == 0) {
                state = std::make_shared<PlayingComThreeState>();
            } else {
                state = std::make_shared<PlayingPlayerThreeDiceState>();
            }
            break;
        case 4:
            if (players[playerNumber - 1] == 0) {
                state = std::make_shared<PlayingComFourState>();
            } else {
                state = std::make_shared<PlayingPlayerFourDiceState>();
            }
            break;
    }
}

void ServerGameCommunicator::resetPlayersArrayFromQuitting () {
    for (int playerArrayIndex = 0; playerArrayIndex < 4; ++playerArrayIndex) {
        if (players[playerArrayIndex] == 9) players[playerArrayIndex] = 1;
    }
}

void ServerGameCommunicator::checkQuitting () {
    bool playersWantToQuit = true;
    for (int playerArrayIndex = 0; playerArrayIndex < 4; ++playerArrayIndex) {
        if (players[playerArrayIndex] == 1) {
            playersWantToQuit = false;
            break;
        }
    }
    if (playersWantToQuit) {
        auto currentTime_t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        char stringDateBuffer[100] = {0};
        std::strftime(stringDateBuffer, sizeof(stringDateBuffer), "%Y-%m-%d-%H:%M:%S", std::localtime(&currentTime_t));
        std::string fileName(stringDateBuffer);
        game->saveGameInFile(fileName);
        exit(0);
    }
}

/*
Every answer has a prefix to state if a state has to be changed after the action was performed.
xx: No state change
[1-4]n: Next Player [1-4]
[1-4]m: Player [1-4] choose Meeple
[1-4]h: Player [1-4] roll the dice again because he/she got out of the starting house.
*/
std::string GameStartState::reactToPlayerInput (std::string input, MadnGame_Ptr game) {
    std::regex joinExpression("[1-4](join)");
    std::regex startExpression("[1-4]start");
    if (std::regex_match(input, joinExpression)) {
        return "xx|" + input.substr(0, 1);
    } else if (std::regex_match(input, startExpression)) {
        return "xx|" + input.substr(0, 1) + "s";
    } else {
        return "xx|inputDoesNotMatchState";
    }
}

std::string PlayingPlayerOneDiceState::reactToPlayerInput (std::string input, MadnGame_Ptr game) {
    std::regex diceExpression("[1-4]D");
    std::regex quitExpression("[1-4]quit");
    if (input.compare("N") == 0) return "xx|" + game->getBoardAsString();
    else if (std::regex_match(input, diceExpression)) {
        if (input.compare("1D") == 0) {
            game->rollTheDice();
            return "1m|" + game->getBoardAsString();
        } else return "xx|notYourTurn";
    } else if (std::regex_match(input, quitExpression)) {
        std::string quitString = "xx|";
        return quitString.append(input.substr(0, 1)).append("q");
    } else return "xx|inputDoesNotMatchState";
}

std::string PlayingPlayerTwoDiceState::reactToPlayerInput (std::string input, MadnGame_Ptr game) {
    std::regex diceExpression("[1-4]D");
    std::regex quitExpression("[1-4]quit");
    if (input.compare("N") == 0) return "xx|" + game->getBoardAsString();
    else if (std::regex_match(input, diceExpression)) {
        if (input.compare("2D") == 0) {
            game->rollTheDice();
            return "2m|" + game->getBoardAsString();
        } else return "xx|notYourTurn";
    } else if (std::regex_match(input, quitExpression)) {
        std::string quitString("xx|" + std::stoi(input.substr(0, 1)));
        return quitString.append("q");
    } else return "xx|inputDoesNotMatchState";
}

std::string PlayingPlayerThreeDiceState::reactToPlayerInput (std::string input, MadnGame_Ptr game) {
    std::regex diceExpression("[1-4]D");
    std::regex quitExpression("[1-4]quit");
    if (input.compare("N") == 0) return "xx|" + game->getBoardAsString();
    else if (std::regex_match(input, diceExpression)) {
        if (input.compare("3D") == 0) {
            game->rollTheDice();
            return "3m|" + game->getBoardAsString();
        } else return "xx|notYourTurn";
    } else if (std::regex_match(input, quitExpression)) {
        std::string quitString("xx|" + std::stoi(input.substr(0, 1)));
        return quitString.append("q");
    } else return "xx|inputDoesNotMatchState";
}

std::string PlayingPlayerFourDiceState::reactToPlayerInput (std::string input, MadnGame_Ptr game) {
    std::regex diceExpression("[1-4]D");
    std::regex quitExpression("[1-4]quit");
    if (input.compare("N") == 0) return "xx|" + game->getBoardAsString();
    else if (std::regex_match(input, diceExpression)) {
        if (input.compare("4D") == 0) {
            game->rollTheDice();
            return "4m|" + game->getBoardAsString();
        } else return "xx|notYourTurn";
    } else if (std::regex_match(input, quitExpression)) {
        std::string quitString("xx|" + std::stoi(input.substr(0, 1)));
        return quitString.append("q");
    } else return "xx|inputDoesNotMatchState";
}

std::string PlayingPlayerOneMeepleState::reactToPlayerInput (std::string input, MadnGame_Ptr game) {
    std::regex chooseMeeple("[1-4][0-4]");
    std::regex skipMove("[1-4]P");
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
                return "1h|" + game->getBoardAsString();
            } else {
                return "xx|moveNotPossible";
            }
        }
    } else if (std::regex_match(input, skipMove)) {
        return "2n|" + game->getBoardAsString();
    } else return "xx|inputDoesNotMatchState";
}

std::string PlayingPlayerTwoMeepleState::reactToPlayerInput (std::string input, MadnGame_Ptr game) {
    std::regex chooseMeeple("[1-4][0-4]");
    std::regex skipMove("[1-4]P");
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
                return "2h|" + game->getBoardAsString();
            } else {
                return "xx|moveNotPossible";
            }
        }
    } else if (std::regex_match(input, skipMove)) {
        return "3n|" + game->getBoardAsString();
    } else return "xx|inputDoesNotMatchState";
}

std::string PlayingPlayerThreeMeepleState::reactToPlayerInput (std::string input, MadnGame_Ptr game) {
    std::regex chooseMeeple("[1-4][0-4]");
    std::regex skipMove("[1-4]P");
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
                return "3h|" + game->getBoardAsString();
            } else {
                return "xx|moveNotPossible";
            }
        }
    } else if (std::regex_match(input, skipMove)) {
        return "4n|" + game->getBoardAsString();
    } else return "xx|inputDoesNotMatchState";
}

std::string PlayingPlayerFourMeepleState::reactToPlayerInput (std::string input, MadnGame_Ptr game) {
    std::regex chooseMeeple("[1-4][0-4]");
    std::regex skipMove("[1-4]P");
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
                return "4h|" + game->getBoardAsString();
            } else {
                return "xx|moveNotPossible";
            }
        }
    } else if (std::regex_match(input, skipMove)) {
        return "1n|" + game->getBoardAsString();
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

std::string PlayingPlayerOneDiceOutOfHouseState::reactToPlayerInput (std::string input, MadnGame_Ptr game) {
    std::cout << "In out of house 1" << std::endl;
    std::regex diceExpression("[1-4]D");
    if (input.compare("N") == 0) return "xx|" + game->getBoardAsString();
    else if (std::regex_match(input, diceExpression)) {
        int playerNumber = std::stoi(input.substr(0, 1));
        if (input.compare("1D") == 0) {
            game->rollTheDice();
            game->movePlayerByPlayerNumberAndDiceNumberAndMeepleNumber(playerNumber, 5);
            return "2n|" + game->getBoardAsString();
        } else return "xx|notYourTurn";
    } else return "xx|inputDoesNotMatchState";
}

std::string PlayingPlayerTwoDiceOutOfHouseState::reactToPlayerInput (std::string input, MadnGame_Ptr game) {
        std::cout << "In out of house 2" << std::endl;

    std::regex diceExpression("[1-4]D");
    if (input.compare("N") == 0) return "xx|" + game->getBoardAsString();
    else if (std::regex_match(input, diceExpression)) {
        int playerNumber = std::stoi(input.substr(0, 1));
        if (input.compare("2D") == 0) {
            game->rollTheDice();
            game->movePlayerByPlayerNumberAndDiceNumberAndMeepleNumber(playerNumber, 5);
            return "3n|" + game->getBoardAsString();
        } else return "xx|notYourTurn";
    } else return "xx|inputDoesNotMatchState";
} 

std::string PlayingPlayerThreeDiceOutOfHouseState::reactToPlayerInput (std::string input, MadnGame_Ptr game) {
        std::cout << "In out of house 3" << std::endl;

    std::regex diceExpression("[1-4]D");
    if (input.compare("N") == 0) return "xx|" + game->getBoardAsString();
    else if (std::regex_match(input, diceExpression)) {
        int playerNumber = std::stoi(input.substr(0, 1));
        if (input.compare("3D") == 0) {
            game->rollTheDice();
            game->movePlayerByPlayerNumberAndDiceNumberAndMeepleNumber(playerNumber, 5);
            return "4n|" + game->getBoardAsString();
        } else return "xx|notYourTurn";
    } else return "xx|inputDoesNotMatchState";
} 

std::string PlayingPlayerFourDiceOutOfHouseState::reactToPlayerInput (std::string input, MadnGame_Ptr game) {
        std::cout << "In out of house 4" << std::endl;

    std::regex diceExpression("[1-4]D");
    if (input.compare("N") == 0) return "xx|" + game->getBoardAsString();
    else if (std::regex_match(input, diceExpression)) {
        int playerNumber = std::stoi(input.substr(0, 1));
        if (input.compare("4D") == 0) {
            game->rollTheDice();
            game->movePlayerByPlayerNumberAndDiceNumberAndMeepleNumber(playerNumber, 5);
            return "1n|" + game->getBoardAsString();
        } else return "xx|notYourTurn";
    } else return "xx|inputDoesNotMatchState";
} 