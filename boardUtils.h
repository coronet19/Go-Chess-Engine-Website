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
    bool hasMoved;       // Required for pawns/castling
    bool isProtected;    // Required for king moveset
    bool isPessantable;  // Required for en passant

    Square(Piece p = Piece::none, Color c = Color::none, std::vector<std::pair<int, int>> m = {}, bool moved = false, bool protected_ = false, bool pessantable = false)
        : piece(p), color(c), moves(m), hasMoved(moved), isProtected(protected_), isPessantable(pessantable) {}
};

#endif // BOARD_UTILS_H
