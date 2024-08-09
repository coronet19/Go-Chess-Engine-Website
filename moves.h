#ifndef MOVES_H
#define MOVES_H

#include <vector>
#include "boardUtils.h"

using namespace std;




class Moves {
public:
    static void calculateMoves(Square (*board)[8]);
    static void resolveKingMoves(Square (*board)[8], pair<int, int> blackKingPosition, pair<int, int> whiteKingPosition);

private:
    static vector<pair<int, int>> getMoves(Square (*board)[8], int row, int col);
    static vector<pair<int, int>> getPawnMoves(Square (*board)[8], int row, int col);
    static vector<pair<int, int>> getRookMoves(Square (*board)[8], int row, int col);
    static vector<pair<int, int>> getKnightMoves(Square (*board)[8], int row, int col);
    static vector<pair<int, int>> getBishopMoves(Square (*board)[8], int row, int col);
    static vector<pair<int, int>> getQueenMoves(Square (*board)[8], int row, int col);
    static vector<pair<int, int>> getKingMoves(Square (*board)[8], int row, int col);
};

#endif // MOVES_H
