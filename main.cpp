#include "board.cpp"

using namespace std;




int main(){
    Board* game = new Board();

    game->printBoard();
    cout << game->toString();

    return 0;
}