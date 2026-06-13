
#include "stdafx.h"
#include "Game.h"

using namespace std;

// Initialize window settings
void Game::initWindow() 
{
    this->window.create(sf::VideoMode(800, 600), "Mario on its peak", sf::Style::Close | sf::Style::Titlebar);
    this->window.setFramerateLimit(60);
}
void Game :: initPlayer (){
    this -> player = new Player();
}


Game::Game() {
    this->initWindow();
    this->initPlayer();
}

// Destructor
Game::~Game() {
    delete this -> player;
}

// Accessor for the window
const sf::RenderWindow& Game::getwindow() const {
    return this->window;
}

// Update game logic and handle events
void Game::update() {
    // polling window event 
    while (window.pollEvent(this->event))
    {
        if (this ->event.type == sf :: Event :: Closed)
        {
            window.close();
        }else if (this->event.type == sf :: Event :: KeyPressed && this->event.key.code == sf :: Keyboard :: Escape){

        }
        
    }

    updatePlayer();
    
}

// Render game objects (draw to the window)
void Game::render() {
this -> window.clear(sf::Color :: Blue);
// render the game
this -> renderPlayer();
this -> window.display();
}

void Game :: updatePlayer(){
    this-> player->update();
}

void Game :: renderPlayer (){
    this->player->render(this->window);
}