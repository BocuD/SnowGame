#include <LDtkLoader/Project.hpp>
#include <SFML/Graphics.hpp>

#include "Scene.h"
#include "Fonts.h"
#include "Game.h"
#include "Player.h"
#include "ParallaxBackground.h"

Game* game;

int main(char* args)
{
    sf::RenderWindow window(sf::VideoMode(1280, 720), "Snow Game");
    window.setVerticalSyncEnabled(true);

    game = new Game;
    game->init();

    sf::Clock timer;
    timer.restart();

    while (window.isOpen()) {

        sf::Event e;
        while (window.pollEvent(e)) 
        {
            switch(e.type)
            {
            case sf::Event::EventType::Closed:
                window.clear();
                window.close();
                return 0;
            default:
                game->eventHandler(e);
                break;
            }
        }

        window.clear();

        game->update();
        game->draw(&window);

        //draw fps counter in window title
        std::string windowTitle = "Snow Game -- FPS: " + 
            std::to_string(1000000 / timer.getElapsedTime().asMicroseconds());
        window.setTitle(windowTitle);
        timer.restart();

        window.display();
    }
    return 0;
}