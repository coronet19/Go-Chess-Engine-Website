#include <stdio.h>
#include <iostream>
#include <vector>

using namespace std;


enum Piece{
    empty, pawn, rook, knight, bishop, queen, king
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
    bool pessantable;  // required for en pessant
};


class Game{
private:
    Square board[8][8];

public:
    Game(){
        // initializes the board with the starting pieces
        for(int row = 0; row < 8; row++){
            for(int col = 0; col < 8; col++){
                if(row == 1){
                    board[row][col] = { Piece::pawn, Color::black, { {}, {} }, false };
                } else if(row == 6){
                    board[row][col] = { Piece::pawn, Color::white, { {}, {} }, false };
                } else{
                    board[row][col] = { Piece::empty, Color::none };
                }
            }
        }

        board[0][0] = { rook, black, {}, false };
        board[0][1] = { knight, black, {}, false };
        board[0][2] = { bishop, black, {}, false };
        board[0][3] = { queen, black, {}, false };
        board[0][4] = { king, black, {}, false };
        board[0][5] = { bishop, black, {}, false };
        board[0][6] = { knight, black, {}, false };
        board[0][7] = { rook, black, {}, false };

        board[7][0] = { rook, white, {}, false };
        board[7][1] = { knight, white, {}, false };
        board[7][2] = { bishop, white, {}, false };
        board[7][3] = { queen, white, {}, false };
        board[7][4] = { king, white, {}, false };
        board[7][5] = { bishop, white, {}, false };
        board[7][6] = { knight, white, {}, false };
        board[7][7] = { rook, white, {}, false };

        calculateMoves();
    }


    void calculateMoves(){
        for(int row = 0; row < 8; row++){
            for(int col = 0; col < 8; col++){
                board[row][col].moves = Moves::getMoves(board, row, col);
            }
        }
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
    static vector<pair<int, int>> getMoves(Square (*board)[8], int row, int col){
        Square square = board[row][col];

        switch(square.piece){
            case pawn: return getPawnMoves(board, row, col);
            case rook: return getRookMoves(board, row, col);
            case knight: return getKnightMoves(board, row, col);
            case bishop: return getBishopMoves(board, row, col);
            case queen: return getQueenMoves(board, row, col);
            case king: return getKingMoves(board, row, col);
            default: return {};
        }
    }


    static vector<pair<int, int>> getPawnMoves(Square (*board)[8], int row, int col){
        vector<pair<int, int>> res;
        Square square = board[row][col];
        Color oppositeColor = (square.color == black) ? Color::white : Color::black;
        int offset = (square.color == Color::black) ? 1 : -1;
        
        for(int i = 1; i < 2 + square.hasMoved; i++){
            int newRow = row + (i * offset);
            int newCol = col + (i * offset);

            if(newRow < 0 || newRow >= 8 || newCol < 0 || newCol >= 8){
                break;
            }

            Square curr = board[newRow][newCol];
            if(curr.piece == Piece::empty){
                res.push_back({newRow, newCol});
            } else{
                break;
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
        Square square = board[row][col];

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
        Color oppositeColor = (square.color == black) ? Color::white : Color::black;
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

        for(int y = 0; y < 8; y++){
            for(int x = 0; x < 8; x++){
                Square curr = board[y][x];

                if(curr.color == Color::none){
                    res.push_back({y, x});
                } else if(curr)
            }
        }
    }
};



int main(){
    Game* game = new Game();

    game->printBoard();
    cout << game->toString();

    return 0;
}