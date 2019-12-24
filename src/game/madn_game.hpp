#include <iostream>

class MenschAergereDichNichtGame {
    private:
        int8_t* board;
        void checkWinner ();
    public:
        MenschAergereDichNichtGame () : board(new int8_t[63]) {}
        ~MenschAergereDichNichtGame () {
            delete[] board;
        }
        int8_t* getBoard () {return board;};
        std::string getBoardAsString ();
        void movePlayerByPlayerNumberAndDiceNumberAndMeepleNumber (int8_t playerNumber, int8_t diceNumber, int8_t figureNumber);
        // Meeple number is determined by the position on the field. 0 = meeple in starting house. 1-4 = meeple in clockwise order
        // on the field, beginning with the yellow starting field.
};