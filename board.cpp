#include <vector>

#include "moves.cpp"
#include "boardUtils.h"
#include "board.h"

using namespace std;




Board::Board() {
    for(int row = 0; row < 8; row++){
        for(int col = 0; col < 8; col++){
            switch(row){
                case 1:  board[row][col] = Square(Piece::pawn, Color::black); break;
                case 6:  board[row][col] = Square(Piece::pawn, Color::white); break;
                default: board[row][col] = Square(); break;
            }
        }
    }

    board[0][0] = Square(Piece::rook, Color::black);
    board[0][1] = Square(Piece::knight, Color::black);
    board[0][2] = Square(Piece::bishop, Color::black);
    board[0][3] = Square(Piece::queen, Color::black);
    board[0][4] = Square(Piece::king, Color::black);
    board[0][5] = Square(Piece::bishop, Color::black);
    board[0][6] = Square(Piece::knight, Color::black);
    board[0][7] = Square(Piece::rook, Color::black);

    board[7][0] = Square(Piece::rook, Color::white);
    board[7][1] = Square(Piece::knight, Color::white);
    board[7][2] = Square(Piece::bishop, Color::white);
    board[7][3] = Square(Piece::queen, Color::white);
    board[7][4] = Square(Piece::king, Color::white);
    board[7][5] = Square(Piece::bishop, Color::white);
    board[7][6] = Square(Piece::knight, Color::white);
    board[7][7] = Square(Piece::rook, Color::white);

    Moves::calculateMoves(board);
}


// converts boardstate to a string
// pieces are separated by a comma ','
// blanks are represented as two periods ".."
string Board::toString(){
    string res = "";
    char piece;
    char color;

    for(auto& row : board){
        for(auto& square : row){
            switch (square.piece){
                case Piece::pawn: piece = 'P'; break;
                case Piece::rook: piece = 'R'; break;
                case Piece::knight: piece = 'N'; break;
                case Piece::bishop: piece = 'B'; break;
                case Piece::queen: piece = 'Q'; break;
                case Piece::king: piece = 'K'; break;
                default: piece = '.'; break;
            }

            switch(square.color){
                case Color::white: color = 'W'; break;
                case Color::black: color = 'B'; break;
                default: color = '.'; break; 
            }

            res = res + color + piece + ',';
        }
    }

    res.pop_back();
    return res;
}


// prints board to stdout
void Board::printBoard(){
    for(auto& row : board){
        for(auto& square : row){
            switch(square.color){
                case Color::white: cout << 'W'; break;
                case Color::black: cout << 'B'; break;
                default: cout << ' '; break;
            }

            switch (square.piece){
                case Piece::pawn: cout << 'P'; break;
                case Piece::rook: cout << 'R'; break;
                case Piece::knight: cout << 'N'; break;
                case Piece::bishop: cout << 'B'; break;
                case Piece::queen: cout << 'Q'; break;
                case Piece::king: cout << 'K'; break;
                default: cout << ' '; break;
            }

            cout << ' ';
        }

        cout << endl;
    }
}