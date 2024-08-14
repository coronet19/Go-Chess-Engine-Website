#ifndef BOARD_UTILS_H
#define BOARD_UTILS_H

#include <vector>


// Enums for the pieces and colors
enum class Piece {
    none, pawn, rook, knight, bishop, queen, king
};


enum class Color {
    none, white, black
};


// Struct representing a square on the chessboard
struct Square {
    Piece piece;
    Color color;
    std::vector<std::pair<int, int>> moves;
    bool hasMoved;       // required for castling
    bool isProtected;    // Required for king moveset
    bool isPessantable;  // Required for en passant

    Square(Piece p = Piece::none, Color c = Color::none, std::vector<std::pair<int, int>> m = {}, bool moved = false, bool prot = false, bool pessantable = false)
        : piece(p), color(c), moves(m), hasMoved(moved), isProtected(prot), isPessantable(pessantable) {}

    // Square(Piece p = Piece::none, Color c = Color::none, bool moved = false)
    //     : piece(p), color(c), moves(), hasMoved(moved), isProtected(false), isPessantable(false) {}
};

#endif // BOARD_UTILS_H




#ifndef BOARD_H
#define BOARD_H

#include <iostream>
#include <string>


class Board {
private:
    Square board[8][8];

public:
    Board();
    Board(string boardStr);

    // Converts board state to a string
    // Pieces are separated by a comma ','
    // Blanks are represented as two periods ".."
    std::string toString();

    // Prints board to stdout
    void printBoard();

    void printPieceMoves();
};

#endif // BOARD_H
