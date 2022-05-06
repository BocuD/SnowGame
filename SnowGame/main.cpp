#include <LDtkLoader/Project.hpp>
#include <SFML/Graphics.hpp>

#include "Scene.h"
#include "Game.h"

Game* game;
const float dt = 1/60.f;

int main(char* args)
{
    sf::RenderWindow window(sf::VideoMode(1280, 720), "Snow Game");
    window.setVerticalSyncEnabled(true);

    game = new Game;
    game->init();

    sf::Clock frameClock;
    sf::Clock fixedClock;

    frameClock.restart();
    fixedClock.restart();

    float fixedUpdateTimer = 0.f;

    while (window.isOpen()) 
    {
        float updateTime = frameClock.getElapsedTime().asSeconds();
        frameClock.restart();

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
                game->eventHandler(e, &window);
                break;
            }
        }

        fixedUpdateTimer += fixedClock.getElapsedTime().asSeconds();
        fixedClock.restart();

        while(fixedUpdateTimer >= dt)
        {
            game->fixedUpdate();
            fixedUpdateTimer -= dt;
        }

        window.clear();

        game->update(updateTime);
        game->draw(&window);

        //draw fps counter in window title
        std::string windowTitle = "Snow Game -- FPS: " + 
            std::to_string(1 / updateTime).substr(0, 5);
        window.setTitle(windowTitle);

        window.display();
    }
    return 0;
}