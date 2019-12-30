#ifndef H_MADNGAME
#define H_MADNGAME

#include <iostream>
#include <set>
#include <fstream>
#include <random>
#include <memory>

class MenschAergereDichNichtGame {
    private:
        int8_t* board;
        int8_t checkWinner () {
            // Retruns number code of winner or 0 if there is no winner yet.
            int gelb = 0;
            int blau = 0;
            int rot = 0;
            int gruen = 0;
            for (int houseIndex = 0; houseIndex <= 15; houseIndex = houseIndex + 5) {
                for (int singleFields = 1; singleFields <= 4; ++singleFields) {
                    switch (houseIndex) {
                        case 0:
                            if (board[houseIndex + singleFields] == 1) ++gelb;
                            break;
                        case 5:
                            if (board[houseIndex + singleFields] == 1) ++blau;
                            break;
                        case 10:
                            if (board[houseIndex + singleFields] == 1) ++rot;
                            break;
                        case 15:
                            if (board[houseIndex + singleFields] == 1) ++gruen;
                            break;
                    }
                }
            }
            if (gelb == 4) return 1;
                else if (blau == 4) return 2;
                else if (rot == 4) return 3;
                else if (gruen == 4) return 4;
                
                return 0;
        };
    public:
        MenschAergereDichNichtGame ();
        MenschAergereDichNichtGame (std::string filePath);
        ~MenschAergereDichNichtGame () {
            delete[] board;
        }
        int8_t* getBoard () {return board;};
        std::string getBoardAsString ();
        std::string movePlayerByPlayerNumberAndDiceNumberAndMeepleNumber (int8_t playerNumber, int8_t meepleNumber);
        // Meeple number is determined by the position on the field. 0 = meeple in starting house. 1-4 = meeple in clockwise order
        // on the field, beginning with the yellow starting field.
        void saveGameInFile ();
        void rollTheDice ();
        int8_t getCurrentDiceNumber () {return board[61];};
};

int8_t addOnBoard (int8_t sum1, int8_t sum2);

class MeepleNumberNotFoundException : std::exception {
    public:
        virtual const char* what() const throw() override {
            return "The chosen meeple number is not available.\n";
        }
};

class MoveNotPossibleException : std::exception {
    public:
        virtual const char* what() const throw() override {
            return "This move vialates the rules of the game.\n";
        }
};

using MadnGame_Ptr = std::shared_ptr<MenschAergereDichNichtGame>;

#endif