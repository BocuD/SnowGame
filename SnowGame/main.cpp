#include <cstdio>
#include <LDtkLoader/Project.hpp>
#include <SFML/Graphics.hpp>

#include "AnimatedGameObject.h"
#include "Scene.h"
#include "Fonts.h"
#include "Player.h"

void handleInput(const sf::Event::KeyEvent& key);

int main(char* args)
{
    sf::RenderWindow window(sf::VideoMode(1280, 720), "Snow Game");
    window.setVerticalSyncEnabled(true);

    sf::Clock timer;
    timer.restart();

    Scene scene("Assets/world.ldtk", "World_Level_0");

    Fonts::loadFonts();

    sf::View view({ 0, 0, 1280, 720 });
    view.zoom(0.2f);

    while (window.isOpen()) {

        sf::Event e;
        while (window.pollEvent(e)) {

            switch (e.type) {
            case sf::Event::EventType::Closed:
                window.clear();
                window.close();
                return 0;
            case sf::Event::EventType::Resized:
                view.setSize({ (float)e.size.width, (float)e.size.height });
                view.zoom(0.2f);
                break;
            case sf::Event::KeyPressed:
                handleInput(e.key);
                break;
            }
        }

        window.clear();
        window.setView(view);

        //update the scene
        scene.update();
        scene.draw(&window);

        view.setCenter(scene.player->getPosition());

        //draw fps counter in window title
        std::string windowTitle = "Snow Game -- FPS: " + 
            std::to_string(1000000 / timer.getElapsedTime().asMicroseconds());
        window.setTitle(windowTitle);
        timer.restart();

        window.display();
    }
    return 0;
}

//can be used for gui navigation
void handleInput(const sf::Event::KeyEvent& key)
{
	switch(key.code)
	{
    case sf::Keyboard::Up:
	case sf::Keyboard::W:
		break;
    case sf::Keyboard::Down:
    case sf::Keyboard::S:
		break;
    case sf::Keyboard::Left:
	case sf::Keyboard::A:
		break;
    case sf::Keyboard::Right:
	case sf::Keyboard::D:
		break;

	case sf::Keyboard::Escape: break;
	case sf::Keyboard::LControl: break;
	case sf::Keyboard::LShift: break;
	case sf::Keyboard::RControl: break;
	case sf::Keyboard::RShift: break;
	case sf::Keyboard::RAlt: break;
	case sf::Keyboard::Space: break;
	default: break;
	}
}
