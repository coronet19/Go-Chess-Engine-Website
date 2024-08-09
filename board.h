#ifndef BOARD_H
#define BOARD_H

#include <iostream>
#include <vector>
#include "boardUtils.h"
#include "Moves.h"




class Board {
private:
    Square board[8][8];

public:
    Board();

    // Converts board state to a string
    // Pieces are separated by a comma ','
    // Blanks are represented as two periods ".."
    std::string toString();

    // Prints board to stdout
    void printBoard();
};

#endif // BOARD_H
