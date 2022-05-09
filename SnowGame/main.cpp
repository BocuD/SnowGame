#include <LDtkLoader/Project.hpp>
#include <SFML/Graphics.hpp>

#include "Scene.h"
#include "Game.h"

const float dt = 1/60.f;

int main(char* args)
{
    sf::RenderWindow window(sf::VideoMode(1280, 800), "Snow Game");
    window.setVerticalSyncEnabled(true);

    Game game;
    game.init();

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
                game.eventHandler(e, &window);
                break;
            }
        }

        fixedUpdateTimer += fixedClock.getElapsedTime().asSeconds();
        fixedClock.restart();

        while(fixedUpdateTimer >= dt)
        {
            game.fixedUpdate();
            fixedUpdateTimer -= dt;
        }

        window.clear();

        game.update(updateTime);
        game.draw(&window);

        //draw fps counter in window title
        std::string windowTitle = "Snowball Jump -- FPS: " + 
            std::to_string(1 / updateTime).substr(0, 5);
        window.setTitle(windowTitle);

        window.display();
    }

    return 0;
}