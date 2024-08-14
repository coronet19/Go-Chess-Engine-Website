#include <iostream>

#include "board.cpp"




int main(){
    Board* game = new Board();

    game->printBoard();
    game->printPieceMoves();    
    std::cout << game->toString() << endl;

    delete game;

    return 0;
}