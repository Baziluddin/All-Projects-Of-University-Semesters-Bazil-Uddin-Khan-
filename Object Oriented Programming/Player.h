#pragma once
#include<stdafx.h>

class Player {
    private:
    sf::Sprite sprite;
    sf::Texture texturesheet;

    // functions init
    void initTextures();
    void initSprite();

    // movement

    // animation
    sf :: IntRect currentFrame;

    //core
    public:
    Player();
    ~Player();

    //function 
    void update();
    void render(sf:: RenderTarget &target);
    void updateMovement();
};