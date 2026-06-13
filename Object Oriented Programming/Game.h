#pragma once
#include<stdafx.h>
#include<Player.h>

class Game {

    private:
    sf :: RenderWindow window;// sf :: RenderWindow same as std :: int, string or char it creates a window for us like int creates a vairable 
    sf :: Event event;
    Player*player; // composition  weak dependency 
    void initWindow ();
    void initPlayer();

    public :
    // constructor 
        Game ();
    ~Game ();

    // functions 
    void update();
    void render();
    void updatePlayer();
    void renderPlayer();


    const sf :: RenderWindow & getwindow () const ; // sf :: RenderWindow therefore used as a identifier of return 
};