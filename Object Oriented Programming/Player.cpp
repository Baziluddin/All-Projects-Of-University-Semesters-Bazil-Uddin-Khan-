#include<stdafx.h>
#include<Player.h>
using namespace std;



void Player :: render(sf:: RenderTarget &target){
    target.draw(this->sprite);
}

void Player :: initTextures(){
   if ( this -> texturesheet.loadFromFile("textures\\playersheet.png")){
cout << "error player could not load" << endl;
   }
}

void Player :: initSprite (){
    this->sprite.setTexture(this->texturesheet);
    this->currentFrame = sf :: IntRect(0,0,160,200);
    this->sprite.setTextureRect(this->currentFrame);
    
}

Player :: Player (){
    initTextures();
    initSprite();
}

Player :: ~Player (){

}

void Player :: updateMovement()
{
    if (sf::Keyboard :: isKeyPressed(sf :: Keyboard::Key :: A)) //left
    {
        this ->sprite.move(-1.f,0.f);
    }else if  (sf::Keyboard :: isKeyPressed(sf :: Keyboard::Key :: D))//right
    {
        this->sprite.move(1.f,0.f);
    }

        if (sf::Keyboard :: isKeyPressed(sf :: Keyboard::Key :: W)) //left
    {
        this ->sprite.move(0.f,-1.f);
    }else if  (sf::Keyboard :: isKeyPressed(sf :: Keyboard::Key :: S))//right
    {
        this->sprite.move(0.f,1.f);
    }
}

void Player :: update()
{
    this->updateMovement();
}