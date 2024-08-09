#include "boardUtils.h"
#include "moves.h"
#include <unordered_map>

#include <iostream>

using namespace std;


// --------------------------- TODO ---------------------------
// make function that looks for possible self checks with moves:
//  - check kings rows, cols, and diags for same color pieces,
//  - if same color piece found between king and enemy rook/bishop/queen,
//    strip all moves from that defending piece

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

    resolveConflictingKingMoves(board, blackKing, whiteKing);

    cout << "here0";
    resolveSelfChecks(board, blackKing);
    resolveSelfChecks(board, whiteKing);
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


vector<pair<int, int>> Moves::getPawnMoves(Square (*board)[8], int row, int col){
    vector<pair<int, int>> res;
    Square square = board[row][col];
    Color oppositeColor = (square.color == Color::black) ? Color::white : Color::black;
    int offset = (square.color == Color::black) ? 1 : -1;

    const int directions[2][2] = {
        {1 * offset, -1},
        {1 * offset, 1}
    };
    
    // moving forward
    for(int i = 1; i < 2 + !square.hasMoved; i++){
        int newRow = row + (i * offset);
        int newCol = col;

        if(newRow < 0 || newRow >= 8 || newCol < 0 || newCol >= 8){
            continue;
        }

        Square curr = board[newRow][newCol];
        if(curr.piece == Piece::none){
            res.push_back({newRow, newCol});
        } else{
            break;
        }
    }

    // capturing pieces/en pessant
    for(const auto& dir : directions){
        int newRow = row + dir[0];
        int newCol = col + dir[1];

        if(newRow < 0 || newRow >= 8 || newCol < 0 || newCol >= 8){
            continue;
        }

        Square curr = board[newRow][newCol];
        if((curr.piece == Piece::none && board[row][newCol].isPessantable) || curr.color == oppositeColor){
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
            continue;
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

    const int pawnDirections[2][2] = {
        {1, -1},
        {1, 1}
    };

    for(int newRow = 0; newRow < 8; newRow++){
        for(int newCol = 0; newCol < 8; newCol++){
            Square curr = board[newRow][newCol];
            int offset = (curr.color == Color::black) ? 1 : -1;

            if(curr.color == square.color){
                valid[newRow][newCol] = false;
            } else if(curr.color == oppositeColor && curr.isProtected){
                valid[newRow][newCol] = false;
            }

            if(curr.color == oppositeColor && curr.piece == Piece::pawn){
                for(const auto& dir : pawnDirections){
                    int pawnRow = newRow + (dir[0] * offset);
                    int pawnCol = newCol + (dir[1] * offset);

                    if(pawnRow < 0 || pawnRow >= 8 || pawnCol < 0 || pawnCol >= 8){
                        continue;
                    }

                    valid[pawnRow][pawnCol] = false;
                }
            }
        }
    }

    for(const auto& dir : directions){
        int newRow = row + dir[0];
        int newCol = col + dir[1];

        if(newRow < 0 || newRow >= 8 || newCol < 0 || newCol >= 8){
            continue;
        }

        if(valid[newRow][newCol]){
            res.push_back({newRow, newCol});
        } else if(board[newRow][newCol].color == square.color){
            board[newRow][newCol].isProtected = true;
        }
    }

    return res;
}


// removes similar moves from both kings possible moves
// suboptimal O(n^2) approach but n <= 16 so meh
void Moves::resolveConflictingKingMoves(Square (*board)[8], pair<int, int> blackKingPosition, pair<int, int> whiteKingPosition){
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


// looks for possible self checks with moves:
//  - check kings rows, cols, and diags for same color pieces,
//  - if same color piece found between king and enemy rook/bishop/queen,
//    strip all moves from that defending piece
//
//
// also broken apparently
void Moves::resolveSelfChecks(Square (*board)[8], pair<int, int> kingPosition){
    Square king = board[kingPosition.first][kingPosition.second];
    Color oppositeColor = (king.color == Color::black) ? Color::white : Color::black;

    const int adjacents[4][2] = {   
        {-1, 0},  // North
        {0, 1},   // East
        {1, 0},   // South
        {0, -1}   // West
    };

    const int diagonals[4][2] = {
        {-1, -1}, // Northwest
        {-1, 1},  // Northeast
        {1, -1},  // Southwest
        {1, 1}    // Southeast
    };

    // Check for adjacent (orthogonal) directions
    for (const auto& dir : adjacents){
        vector<pair<int, int>> validMoves;
        Square* defendingPiece = nullptr;
        int offset = 1;

        while(true){
            int newRow = kingPosition.first + offset * dir[0];
            int newCol = kingPosition.second + offset * dir[1];

            if(newRow < 0 || newRow >= 8 || newCol < 0 || newCol >= 8){
                break;
            }

            Square curr = board[newRow][newCol];

            if(curr.color == king.color){
                if(defendingPiece == nullptr){
                    defendingPiece = &board[newRow][newCol];
                }
                offset++;
                continue;
            }

            if(defendingPiece != nullptr && curr.color == oppositeColor && (curr.piece == Piece::rook || curr.piece == Piece::queen)){
                intersectMoves(defendingPiece, validMoves);
                break;
            }

            if(curr.color == oppositeColor){
                break;
            }

            validMoves.push_back({newRow, newCol});
            offset++;
        }
    }

    // Check for diagonal directions
    for(const auto& dir : diagonals){
        vector<pair<int, int>> validMoves;
        Square* defendingPiece = nullptr;
        int offset = 1;

        while(true){
            int newRow = kingPosition.first + offset * dir[0];
            int newCol = kingPosition.second + offset * dir[1];

            if(newRow < 0 || newRow >= 8 || newCol < 0 || newCol >= 8){
                break;
            }

            Square curr = board[newRow][newCol];

            if(curr.color == king.color){
                if(defendingPiece == nullptr){
                    defendingPiece = &board[newRow][newCol];
                }
                offset++;
                continue;
            }

            if(defendingPiece != nullptr && curr.color == oppositeColor && (curr.piece == Piece::bishop || curr.piece == Piece::queen)){
                intersectMoves(defendingPiece, validMoves);
                break;
            }

            if(curr.color == oppositeColor){
                break;
            }

            validMoves.push_back({newRow, newCol});
            offset++;
        }
    }
}
// void Moves::resolveSelfChecks(Square (*board)[8], pair<int, int> kingPosition){
//     Square king = board[kingPosition.first][kingPosition.second];
//     Color oppositeColor = (king.color == Color::black) ? Color::white : Color::black;
//     Square* defendingPiece = nullptr;

//     const int adjacents[4][2] = {
//         {-1, 0},  // North
//         {0, 1},   // East
//         {1, 0},   // South
//         {0, -1}   // West
//     };

//     const int diagonals[4][2] = {
//         {-1, -1}, // Northwest
//         {-1, 1},  // Northeast
//         {1, -1},  // Southwest
//         {1, 1}   // Southeast
//     };

//     for(const auto& dir : adjacents){
//         vector<pair<int, int>> validMoves;
//         int offset = 1;

//         while(true){
//             int newRow = offset * dir[0];
//             int newCol = offset * dir[1];

//             if(newRow < 0 || newRow >= 8 || newCol < 0 || newCol >= 8){
//                 break;
//             }

//             Square curr = board[newRow][newCol];

//             if(defendingPiece == nullptr){
//                 if(curr.color == oppositeColor){
//                     break;
//                 } else if(curr.color == king.color){
//                     defendingPiece = &board[newRow][newCol];
//                     continue;
//                 }
//             } else if(curr.color == oppositeColor && curr.piece == Piece::rook || curr.piece == Piece::queen){
//                 // This breaks black kingside knight on board initialization.
//                 // This code block shouldnt even be reached by anything other
//                 // than 6 specific pawns on board initialization.
//                 intersectMoves(defendingPiece, validMoves); 
//                 break;
//             }

//             offset++;
//             validMoves.push_back({newRow, newCol});
//         }
//     }

//     for(const auto& dir : diagonals){
//         vector<pair<int, int>> validMoves;
//         int offset = 1;

//         while(true){
//             int newRow = offset * dir[0];
//             int newCol = offset * dir[1];

//             if(newRow < 0 || newRow >= 8 || newCol < 0 || newCol >= 8){
//                 break;
//             }
            
//             Square curr = board[newRow][newCol];

//             if(defendingPiece == nullptr){
//                 if(curr.color == oppositeColor){
//                     break;
//                 } else if(curr.color == king.color){
//                     defendingPiece = &board[newRow][newCol];
//                     continue;
//                 }
//             } else if(curr.color == oppositeColor && curr.piece == Piece::bishop || curr.piece == Piece::queen){
//                 intersectMoves(defendingPiece, validMoves); 
//                 break;
//             }

//             offset++;
//             validMoves.push_back({newRow, newCol});
//         }
//     }
// }


// this function does indeed work, look for bugs elsewhere
void Moves::intersectMoves(Square* piece, vector<pair<int, int>> moves){
    vector<pair<int, int>> validMoves;
    vector<bool> map(64, false);

    for(const auto& move : piece->moves){
        int hash = 8 * move.first + move.second;
        
        map[hash] = true;
    }

    for(const auto& move : moves){
        int hash = 8 * move.first + move.second;

        if(map[hash]){
            validMoves.push_back(move);
        }
    }

    piece->moves = validMoves;
}
