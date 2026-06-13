#include "stdafx.h"
#include "Game.h"
#include<Player.h>



using namespace std;

int main() {
    srand(static_cast<unsigned>(time(0)));
    Game game;
    while (game.getwindow().isOpen())// Game will run while window is open
    {
        game.update();
        game.render();
    }
    
    return 0;
}
