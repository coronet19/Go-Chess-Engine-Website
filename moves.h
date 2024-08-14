#ifndef MOVES_H
#define MOVES_H

#include "board.h"

using namespace std;




class Moves {
public:
    static void calculateMoves(Square (*board)[8]);

private:
    static vector<pair<int, int>> getMoves(Square (*board)[8], int row, int col);
    static vector<pair<int, int>> getPawnMoves(Square (*board)[8], int row, int col);
    static vector<pair<int, int>> getRookMoves(Square (*board)[8], int row, int col);
    static vector<pair<int, int>> getKnightMoves(Square (*board)[8], int row, int col);
    static vector<pair<int, int>> getBishopMoves(Square (*board)[8], int row, int col);
    static vector<pair<int, int>> getQueenMoves(Square (*board)[8], int row, int col);
    static vector<pair<int, int>> getKingMoves(Square (*board)[8], int row, int col);
    static void resolveConflictingKingMoves(Square (*board)[8], pair<int, int> blackKingPosition, pair<int, int> whiteKingPosition);
    static void resolveSelfChecks(Square (*board)[8], pair<int, int> kingPosition);
    static void intersectMoves(Square* piece, vector<pair<int, int>> moves);
};

#endif // MOVES_H
