#include "madn_game.hpp"

MenschAergereDichNichtGame::MenschAergereDichNichtGame () : board (new int8_t[63]) {
    std::set<int> startingHouses = {0, 5, 10, 15};
    std::set<int> currentPlayerDiceNumber = {61, 62};

    // Fill in default values for starting a new game into the board array.
    for (int boardIndex = 0; boardIndex < 63; ++boardIndex) {
        if (startingHouses.count(boardIndex) != 0)
            board[boardIndex] = 4;
        else if (currentPlayerDiceNumber.count(boardIndex) != 0)
            board[boardIndex] = 1;
        else
            board[boardIndex] = 0;
    }
}

MenschAergereDichNichtGame::MenschAergereDichNichtGame (std::string filePath) : board (new int8_t[63]) {
    std::ifstream fileInputStream (filePath);
    std::string boardString;
    if (fileInputStream.is_open()) {
        std::getline(fileInputStream, boardString);
    }
    fileInputStream.close();

    // Transfer save game file to board array.
    for (int boardIndex = 0; boardIndex < boardString.size(); ++boardIndex) {
        board[boardIndex] = boardString.at(boardIndex);
    }
}

std::string MenschAergereDichNichtGame::getBoardAsString () {
    std::string returnString = "";

    for (int boardIndex = 0; boardIndex < 63; ++boardIndex) {
        returnString += (board[boardIndex]);
    }

    return returnString;
}

// Returns "succsess" when everything is ok and "win" if one player won. Overwise something different (e.g. a exception message).
std::string MenschAergereDichNichtGame::movePlayerByPlayerNumberAndDiceNumberAndMeepleNumber (int8_t playerNumber, int8_t meepleNumber) {
    // Meeple = "Spielfigur"
    // Dice number is readable from the board inforamtion array. board[62]
    int8_t diceNumber = board[61];

    try {
        if (diceNumber == 6 && board[(playerNumber - 1) * 5] > 0) { // Getting a meeple out of the starting house.
            --board[(playerNumber - 1) * 5]; // Take a meeple out of the players house.
            int playerStartingIndex = 20 + (playerNumber - 1) * 10;
            if (board[playerStartingIndex] != 0) {
                board[(board[playerStartingIndex] - 1) * 5]++;
            }
            board[playerStartingIndex] = playerNumber;
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
                
                if ((index + diceNumber) > (((playerNumber - 1) * 5) + 4)) throw me; // Dice number to large
                for (int jump = 1; jump <= diceNumber; ++jump) {
                    if (board[index + jump] != 0) throw me; // Another meeple in the way.
                }

                board[index + diceNumber] = 1;
                board[index] == 0;
            } else if ((index + diceNumber) > addOnBoard((20 + (playerNumber - 1) * 10), 39)) { // Meeple going into the house from the field.
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
                MoveNotPossibleException me;
                int8_t possibleNewIndex = addOnBoard(index, diceNumber); 
                if (board[possibleNewIndex] == playerNumber) throw me; // You can not throw yourself from the field.
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

void MenschAergereDichNichtGame::saveGameInFile () {
    std::cout << "Wie soll der Speicherstand heißen?" << std::endl;
    std::string fileName;
    std::getline(std::cin, fileName);
    std::ofstream fileOut;
    fileOut.open("../saveGames/" + fileName + ".txt");
    for (int boardIndex = 0; boardIndex < 63; ++boardIndex)
        fileOut << board[boardIndex];
    fileOut.close();
}

// Adding index numbers. Never a higher number than 60; Never lower than 20
int8_t addOnBoard (int8_t sum1, int8_t sum2) {
    if (sum1 + sum2 <= 60) return sum1 + sum2;
    else return 20 + ((sum1 + sum2) - 61);
}