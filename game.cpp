#include <stdio.h>
#include <iostream>
#include <vector>

// #include "game.h"
// #include "board.h"
// #include "moves.h"
// #include "board.cpp"

using namespace std;



enum Piece{
    none, pawn, rook, knight, bishop, queen, king
};


enum Color{
    none, white, black
};


struct Square{
    Piece piece;
    Color color;
    vector<pair<int, int>> moves;
    bool hasMoved;    // required for pawns/castling
    bool isProtected; // required for king moveset
    bool isPessantable;  // required for en pessant


    Square(Piece p = Piece::none, Color c = Color::none, vector<pair<int, int>> m = {}, bool moved = false, bool protected_ = false, bool pessantable = false)
        : piece(p), color(c), moves(m), hasMoved(moved), isProtected(protected_), isPessantable(pessantable) {}
};



class Board{
private:
    Square board[8][8];


public:
    Board(){
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
    string toString(){
        string res = "";
        char piece;
        char color;

        for(auto& row : board){
            for(auto& square : row){
                switch (square.piece){
                    case pawn: piece = 'P'; break;
                    case rook: piece = 'R'; break;
                    case knight: piece = 'N'; break;
                    case bishop: piece = 'B'; break;
                    case queen: piece = 'Q'; break;
                    case king: piece = 'K'; break;
                    default: piece = '.'; break;
                }

                switch(square.color){
                    case white: color = 'W'; break;
                    case black: color = 'B'; break;
                    default: color = '.'; break; 
                }

                res = res + color + piece + ',';
            }
        }

        res.pop_back();
        return res;
    }


    // prints board to stdout
    void printBoard(){
        for(auto& row : board){
            for(auto& square : row){
                switch(square.color){
                    case white: cout << 'W'; break;
                    case black: cout << 'B'; break;
                    default: cout << ' '; break;
                }

                switch (square.piece){
                    case pawn: cout << 'P'; break;
                    case rook: cout << 'R'; break;
                    case knight: cout << 'N'; break;
                    case bishop: cout << 'B'; break;
                    case queen: cout << 'Q'; break;
                    case king: cout << 'K'; break;
                    default: cout << ' '; break;
                }

                cout << ' ';
            }

            cout << endl;
        }
    }
};



class Moves{
public:
    static void calculateMoves(Square (*board)[8]){
        pair<int, int> whiteKing;
        pair<int, int> blackKing;

        for(int row = 0; row < 8; row++){
            for(int col = 0; col < 8; col++){
                board[row][col].isProtected = false;
            }
        }

        for(int row = 0; row < 8; row++){
            for(int col = 0; col < 8; col++){
                if(board[row][col].piece == Piece::king){
                    if(board[row][col].color == Color::black){
                        blackKing = {row, col};
                    } else{
                        whiteKing = {row, col};
                    }
                } else{
                    board[row][col].moves = Moves::getMoves(board, row, col);
                }
            }
        }

        Moves::getMoves(board, whiteKing.first, whiteKing.second);
        Moves::getMoves(board, blackKing.first, blackKing.second);

        Moves::resolveKingMoves(board, blackKing, whiteKing);
    }


    // removes similar moves from both kings possible moves
    // suboptimal O(n^2) approach but 16^2 is small so meh
    static void resolveKingMoves(Square (*board)[8], pair<int, int> blackKingPosition, pair<int, int> whiteKingPosition){
        Square blackKing = board[blackKingPosition.first][blackKingPosition.second];
        Square whiteKing = board[whiteKingPosition.first][whiteKingPosition.second];

        vector<pair<int, int>> newBlackKingMoves;
        vector<pair<int, int>> newWhiteKingMoves;

        for(const auto& blackMove : blackKing.moves){
            for(const auto& whiteMove : whiteKing.moves){
                if(blackMove != whiteMove){
                    newBlackKingMoves.push_back(blackMove);
                }
            }
        }

        for(const auto& whiteMove : whiteKing.moves){
            for(const auto& blackMove : blackKing.moves){
                if(whiteMove != blackMove){
                    newWhiteKingMoves.push_back(whiteMove);
                }
            }
        }

        board[blackKingPosition.first][blackKingPosition.second].moves = newBlackKingMoves;
        board[whiteKingPosition.first][whiteKingPosition.second].moves = newWhiteKingMoves;
    }


private:
    static vector<pair<int, int>> getMoves(Square (*board)[8], int row, int col){
        Square square = board[row][col];

        switch(square.piece){
            case Piece::pawn: return getPawnMoves(board, row, col);
            case Piece::rook: return getRookMoves(board, row, col);
            case Piece::knight: return getKnightMoves(board, row, col);
            case Piece::bishop: return getBishopMoves(board, row, col);
            case Piece::queen: return getQueenMoves(board, row, col);
            case Piece::king: return getKingMoves(board, row, col);
            default: return {};
        }
    }


    static vector<pair<int, int>> getPawnMoves(Square (*board)[8], int row, int col){
        vector<pair<int, int>> res;
        Square square = board[row][col];
        Color oppositeColor = (square.color == Color::black) ? Color::white : Color::black;
        int offset = (square.color == Color::black) ? 1 : -1;
        
        for(int i = 1; i < 2 + square.hasMoved; i++){
            int newRow = row + (i * offset);
            int newCol = col + (i * offset);

            if(newRow < 0 || newRow >= 8 || newCol < 0 || newCol >= 8){
                break;
            }

            Square curr = board[newRow][newCol];
            if(curr.piece == Piece::none){
                res.push_back({newRow, newCol});
            } else{
                break;
            }
        }

        // for capturing pieces
        const int directions[2][2] = {
            {1 * offset, -1},
            {1 * offset, 1}
        };

        for(const auto& dir : directions){
            int newRow = row + dir[0];
            int newCol = col + dir[1];

            if(newRow < 0 || newRow >= 8 || newCol < 0 || newCol >= 8){
                break;
            }

            Square curr = board[newRow][newCol];
            Square curr = board[newRow][newCol];
            if(curr.color == Color::none || curr.color == oppositeColor){
                res.push_back({newRow, newCol});
            } else{
                board[row][col].isProtected = true;
            }
        }
    }


    static vector<pair<int, int>> getRookMoves(Square (*board)[8], int row, int col){
        vector<pair<int, int>> res;
        Square square = board[row][col];
        Color oppositeColor = (square.color == black) ? Color::white : Color::black;

        const int directions[4][2] = {  
            {-1, 0}, // North
            {0, 1},  // East
            {1, 0},  // South
            {0, -1}  // West
        };

        for(const auto& dir : directions){
            int offset = 1;

            while(true){
                int newRow = row + offset * dir[0];
                int newCol = col + offset * dir[1];

                if(newRow < 0 || newRow >= 8 || newCol < 0 || newCol >= 8){
                    break;
                }

                Square curr = board[newRow][newCol];
                if(curr.color == Color::none){
                    res.push_back({newRow, newCol});
                } else if(curr.color == oppositeColor){
                    res.push_back({newRow, newCol});
                    break;
                } else{
                    board[row][col].isProtected = true;
                    break;
                }

                offset++;
            }
        }

        return res;
    }


    static vector<pair<int, int>> getKnightMoves(Square (*board)[8], int row, int col){
        vector<pair<int, int>> res;
        Square square = board[row][col];
        Color oppositeColor = (square.color == black) ? Color::white : Color::black;

        const int directions[8][2] = {
            {-2, -1},
            {-2, 1},
            {-1, 2},
            {1, 2},
            {2, 1},
            {2, -1},
            {1, -2},
            {-1, -2}
        };

        for(const auto& dir : directions){
            int newRow = row + dir[0];
            int newCol = col + dir[1];

            if(newRow < 0 || newRow >= 8 || newCol < 0 || newCol >= 8){
                break;
            }

            Square curr = board[newRow][newCol];
            if(curr.color == Color::none || curr.color == oppositeColor){
                res.push_back({newRow, newCol});
            } else{
                board[row][col].isProtected = true;
            }
        }

        return res;
    }


    static vector<pair<int, int>> getBishopMoves(Square (*board)[8], int row, int col){
        vector<pair<int, int>> res;
        Square square = board[row][col];
        Color oppositeColor = (square.color == black) ? Color::white : Color::black;

        const int directions[4][2] = {
            {-1, -1}, // Northwest
            {-1, 1},  // Northeast
            {1, -1},  // Southwest
            {1, 1}  // Southeast
        };

        for(const auto& dir : directions){
            int offset = 1;

            while(true){
                int newRow = row + offset * dir[0];
                int newCol = col + offset * dir[1];

                if(newRow < 0 || newRow >= 8 || newCol < 0 || newCol >= 8){
                    break;
                }

                Square curr = board[newRow][newCol];
                if(curr.color == Color::none){
                    res.push_back({newRow, newCol});
                } else if(curr.color == oppositeColor){
                    res.push_back({newRow, newCol});
                    break;
                } else{
                    board[row][col].isProtected = true;
                    break;
                }

                offset++;
            }
        }

        return res;
    }


    static vector<pair<int, int>> getQueenMoves(Square (*board)[8], int row, int col){
        vector<pair<int, int>> rookMoves = getRookMoves(board, row, col);
        vector<pair<int, int>> bishopMoves = getBishopMoves(board, row, col);
        
        rookMoves.insert(rookMoves.end(), bishopMoves.begin(), bishopMoves.end());
        return rookMoves;
    }


    static vector<pair<int, int>> getKingMoves(Square (*board)[8], int row, int col){
        vector<pair<int, int>> res;
        Square square = board[row][col];
        Color oppositeColor = (square.color == Color::black) ? Color::white : Color::black;
        bool valid[8][8] = {true};

        const int directions[8][2] = {
            {-1, -1}, // Northwest
            {-1, 0},  // North
            {-1, 1},  // Northeast
            {0, 1},   // East
            {1, 1},   // Southeast
            {1, 0},   // South
            {1, -1},  // Southwest
            {0, -1}   // West
        };

        for(int newRow = 0; newRow < 8; newRow++){
            for(int newCol = 0; newCol < 8; newCol++){
                Square curr = board[newRow][newCol];

                if(curr.color == square.color){
                    valid[newRow][newCol] = false;
                } else if(curr.color == oppositeColor && curr.isProtected){
                    valid[newRow][newCol] = false;
                }
            }
        }

        for(const auto& dir : directions){
            int newRow = row + dir[0];
            int newCol = col + dir[1];

            if(newRow < 0 || newRow >= 8 || newCol < 0 || newCol >= 8){
                break;
            }

            if(valid[newRow][newCol]){
                res.push_back({newRow, newCol});
            } 
        }

        return res;
    }
};









int main(){
    Board* game = new Board();

    game->printBoard();
    cout << game->toString();

    return 0;
}