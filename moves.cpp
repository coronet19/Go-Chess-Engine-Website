#include <vector>

#include "boardUtils.h"
#include "moves.h"

using namespace std;




void Moves::calculateMoves(Square (*board)[8]) {
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
                board[row][col].moves = getMoves(board, row, col);
            }
        }
    }

    getMoves(board, whiteKing.first, whiteKing.second);
    getMoves(board, blackKing.first, blackKing.second);

    resolveKingMoves(board, blackKing, whiteKing);
}


// removes similar moves from both kings possible moves
// suboptimal O(n^2) approach but n <= 16 so meh
void Moves::resolveKingMoves(Square (*board)[8], pair<int, int> blackKingPosition, pair<int, int> whiteKingPosition){
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


vector<pair<int, int>> Moves::getMoves(Square (*board)[8], int row, int col){
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


// unfinished, add en pessant
vector<pair<int, int>> Moves::getPawnMoves(Square (*board)[8], int row, int col){
    vector<pair<int, int>> res;
    Square square = board[row][col];
    Color oppositeColor = (square.color == Color::black) ? Color::white : Color::black;
    int offset = (square.color == Color::black) ? 1 : -1;
    
    // for moving forward
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
        if(curr.color == Color::none || curr.color == oppositeColor){
            res.push_back({newRow, newCol});
        } else{
            board[row][col].isProtected = true;
        }
    }

    return res;
}


vector<pair<int, int>> Moves::getRookMoves(Square (*board)[8], int row, int col){
    vector<pair<int, int>> res;
    Square square = board[row][col];
    Color oppositeColor = (square.color == Color::black) ? Color::white : Color::black;

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


vector<pair<int, int>> Moves::getKnightMoves(Square (*board)[8], int row, int col){
    vector<pair<int, int>> res;
    Square square = board[row][col];
    Color oppositeColor = (square.color == Color::black) ? Color::white : Color::black;

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


vector<pair<int, int>> Moves::getBishopMoves(Square (*board)[8], int row, int col){
    vector<pair<int, int>> res;
    Square square = board[row][col];
    Color oppositeColor = (square.color == Color::black) ? Color::white : Color::black;

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


vector<pair<int, int>> Moves::getQueenMoves(Square (*board)[8], int row, int col){
    vector<pair<int, int>> rookMoves = getRookMoves(board, row, col);
    vector<pair<int, int>> bishopMoves = getBishopMoves(board, row, col);
    
    rookMoves.insert(rookMoves.end(), bishopMoves.begin(), bishopMoves.end());
    return rookMoves;
}


// unfinished, account for pawn movement
vector<pair<int, int>> Moves::getKingMoves(Square (*board)[8], int row, int col){
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
        } else if(board[newRow][newCol].color == square.color){
            board[newRow][newCol].isProtected = true;
        }
    }

    return res;
}
