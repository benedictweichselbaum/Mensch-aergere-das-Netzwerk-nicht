#include "madn_game.hpp"

// Create new game with default values.
MenschAergereDichNichtGame::MenschAergereDichNichtGame () : board (new int8_t[63]) {
    std::set<int> startingHouses = {0, 5, 10, 15};

    // Fill in default values for starting a new game into the board array.
    for (int boardIndex = 0; boardIndex < 63; ++boardIndex) {
        if (startingHouses.count(boardIndex) != 0)
            board[boardIndex] = 4;
        else if (boardIndex == 60) // Current player
            board[boardIndex] = 1;
        else
            board[boardIndex] = 0;
    }
}

// Create game from a save game file.
MenschAergereDichNichtGame::MenschAergereDichNichtGame (std::string filePath) : board (new int8_t[63]) {
    std::ifstream fileInputStream (filePath);
    std::string boardString;
    if (fileInputStream.is_open()) {
        std::getline(fileInputStream, boardString);
    }
    std::cout << boardString << std::endl;
    fileInputStream.close();

    // Transfer save game file to board array.
    for (int boardIndex = 0; boardIndex < boardString.size(); ++boardIndex) {
        board[boardIndex] = std::stoi(boardString.substr(boardIndex, 1));
    }
}

std::string MenschAergereDichNichtGame::getBoardAsString () {
    std::string returnString = "";

    for (int boardIndex = 0; boardIndex < 63; ++boardIndex) {
        returnString.append(std::to_string(board[boardIndex]));
    }

    return returnString;
}

// Returns "succsess" when everything is ok, "again" when a player put a meeple out of his house
// and "win" if one player won. Overwise something different (e.g. a exception message).
std::string MenschAergereDichNichtGame::movePlayerByPlayerNumberAndDiceNumberAndMeepleNumber (int8_t playerNumber, int8_t meepleNumber) {
    // Meeple = "Spielfigur"
    // Dice number is readable from the board inforamtion array. board[62]
    int8_t diceNumber = board[61];

    try {
        if (meepleNumber == 5) { // Special move after a meeple left the starting house.
            int startingIndex = (playerNumber - 1) * 10 + 20;
            int possibleNewIndex = addOnBoard(startingIndex, diceNumber);
            if (board[possibleNewIndex] == playerNumber) { // You can not throw yourself -> New dice roll.
                rollTheDice();
                return this->movePlayerByPlayerNumberAndDiceNumberAndMeepleNumber(playerNumber, 5);
            } else if (board[possibleNewIndex] != 0) {
                ++board[(board[possibleNewIndex] - 1) * 5]; // Put opponent meeple back in his/her starting house.
                board[possibleNewIndex] = playerNumber;
                board[startingIndex] = 0;
            } else {
                board[possibleNewIndex] = playerNumber;
                board[startingIndex] = 0;
            }
            return "success";
        } else if (diceNumber == 6 && board[(playerNumber - 1) * 5] > 0) { // Getting a meeple out of the starting house.
            --board[(playerNumber - 1) * 5]; // Take a meeple out of the players house.
            int playerStartingIndex = 20 + (playerNumber - 1) * 10;
            if (board[playerStartingIndex] != 0) {
                board[(board[playerStartingIndex] - 1) * 5]++;
            }
            board[playerStartingIndex] = playerNumber;
            return "again";
        } else if (meepleNumber >= 1 && meepleNumber <= 4) { // Normal move of meeple on field.
            //Search for right index.
            int index;
            int numberOfFoundMeeples = 0;
            for (int boardIndex = 20; boardIndex < 60; ++boardIndex) {
                if (board[boardIndex] == playerNumber) ++numberOfFoundMeeples;
                if (numberOfFoundMeeples == meepleNumber) {
                    index = boardIndex;
                    break;
                }
            }
            std::cout << "Index of chosen meeple " << index << std::endl;
            // Possible meeples in finish.
            if (numberOfFoundMeeples < meepleNumber) {
                int houseIndexOfPlayer = (playerNumber - 1) * 5;
                for (int finishHouseIndexPlus = 1; finishHouseIndexPlus <= 4; ++finishHouseIndexPlus) {
                    if (board[houseIndexOfPlayer + finishHouseIndexPlus] == 1) ++numberOfFoundMeeples;
                    if (numberOfFoundMeeples == meepleNumber) {
                        index = houseIndexOfPlayer + finishHouseIndexPlus;
                        break;
                    }
                }
            }
            MeepleNumberNotFoundException e;
            if (numberOfFoundMeeples != meepleNumber) throw e;

            MoveNotPossibleException me;
            if (index < 20) { // Moving meeple in finish house.
                std::cout << "Move in finish Field" << std::endl;
                if ((index + diceNumber) > (((playerNumber - 1) * 5) + 4)) throw me; // Dice number to large
                for (int jump = 1; jump <= diceNumber; ++jump) {
                    if (board[index + jump] != 0) throw me; // Another meeple in the way.
                }

                board[index + diceNumber] = 1;
                board[index] = 0;
            } else if ((index + diceNumber) > addOnBoard((20 + (playerNumber - 1) * 10), 39) && index <= addOnBoard((20 + (playerNumber - 1) * 10), 39)) { // Meeple going into the house from the field.
                std::cout << "Move to finish Field" << std::endl;
                int8_t fieldIndexBeforeHouse = addOnBoard((20 + (playerNumber - 1) * 10), 39);
                int intoTheFinishingHouse = fieldIndexBeforeHouse - (index + diceNumber);
                int finishHouseStartIndex = (playerNumber - 1) * 5 + 1;
                int movesToFinishingHouse = fieldIndexBeforeHouse - index;
                int movesInFinishingHouse = diceNumber - movesToFinishingHouse;

                if (movesInFinishingHouse > 4) throw me; // Dice munber to large

                for (int jump = 1; jump <= movesInFinishingHouse; ++jump) {
                    if (board[finishHouseStartIndex + jump] != 0) throw me; // Another meeple in the way;
                }

                board[finishHouseStartIndex + (movesInFinishingHouse - 1)] = 1;
                board[index] = 0;
            } else { // Normal move on the field.
                std::cout << "Normal move" << std::endl;
                MoveNotPossibleException me;
                int8_t possibleNewIndex = addOnBoard(index, diceNumber); 
                if (board[possibleNewIndex] == playerNumber) {
                    std::cout << "You are throwing yourself" << std::endl;
                    throw me;
                } // You can not throw yourself from the field.
                else if (board[possibleNewIndex] != 0) { // Move with throwing an opponent from the field.
                    ++board[(board[possibleNewIndex] - 1) * 5]; // Put opponent meeple back in his/her starting house.
                    board[possibleNewIndex] = playerNumber;
                    board[index] = 0;
                } else { // Normal move without throwing.
                    board[possibleNewIndex] = playerNumber;
                    board[index] = 0;
                }
            }
        } else { // meepleNumber not possible
            MeepleNumberNotFoundException e;
            throw e;
        }

        if (board[62] = checkWinner() != 0) return "win";
        else return "success";
    } catch (MeepleNumberNotFoundException& e) {
        return e.what();
    } catch (MoveNotPossibleException& e) {
        return e.what();
    }
}


// Method for writing a save game into a file.
void MenschAergereDichNichtGame::saveGameInFile (std::string fileName) {
    std::string file = "../src/saveGames/";
    file = file.append(fileName);
    std::ofstream fileOut;
    fileOut.open(file);
    if (fileOut.is_open()) {
        for (int boardIndex = 0; boardIndex < 63; ++boardIndex)
            fileOut << std::to_string(board[boardIndex]);
    } else {
        std::cout << "closed" << std::endl;
    }
    fileOut << std::endl;
    fileOut.close();
}

// Special function for a random dice number.
// Has no bias toward lower numbers.
void MenschAergereDichNichtGame::rollTheDice () {
    std::mt19937 rng(time(NULL));
    std::uniform_int_distribution<int8_t> gen(1, 6);
    board[61] = gen(rng);
}

// Adding index numbers. Never a higher number than 59; Never lower than 20
int8_t addOnBoard (int8_t sum1, int8_t sum2) {
    if (sum1 + sum2 <= 59) return sum1 + sum2;
    else return 20 + ((sum1 + sum2) - 60);
}