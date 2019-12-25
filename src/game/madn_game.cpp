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

void MenschAergereDichNichtGame::movePlayerByPlayerNumberAndDiceNumberAndMeepleNumber (int8_t playerNumber, int8_t meepleNumber) {
    // Dice number is readable from the board inforamtion array.

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