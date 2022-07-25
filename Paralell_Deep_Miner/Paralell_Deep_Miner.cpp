#include <iostream>
#include <time.h>
#include "Game.h"

int main()
{
    //Seeding
    srand(static_cast<unsigned int>(time(NULL)));

    Game* newGame = new Game();
    newGame->start();
    delete newGame;
}
