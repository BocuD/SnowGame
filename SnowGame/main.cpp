#include <LDtkLoader/Project.hpp>
#include <SFML/Graphics.hpp>

#include "Scene.h"
#include "Fonts.h"
#include "Hud.h"
#include "Player.h"

void handleInput(const sf::Event::KeyEvent& key);

Scene* scene;
Hud hud;

int main(char* args)
{
    sf::RenderWindow window(sf::VideoMode(1280, 720), "Snow Game");
    window.setVerticalSyncEnabled(true);

    scene = new Scene("Assets/world.ldtk", "World_Level_0");

    hud.init();

    sf::Clock timer;
    timer.restart();

    Fonts::loadFonts();

    sf::View sceneView({ 0, 0, 1280, 720 });
    sf::View hudView({ 0, 0, 1280, 720 });

    sceneView.zoom(0.3f);
	hudView.zoom(1.0f);

    while (window.isOpen()) {

        sf::Event e;
        while (window.pollEvent(e)) {

            switch (e.type) {
            case sf::Event::EventType::Closed:
                window.clear();
                window.close();
                return 0;
            case sf::Event::EventType::Resized:
                sceneView.setSize({ (float)e.size.width, (float)e.size.height });
                hudView.setSize({ (float)e.size.width, (float)e.size.height });
                sceneView.zoom(720 / sceneView.getSize().y * 0.3f);
                hudView.zoom(720 / hudView.getSize().y);
                hudView.setCenter(hudView.getSize().x / 2, hudView.getSize().y / 2);
                break;
            case sf::Event::KeyPressed:
                handleInput(e.key);
                break;
            }
        }

        //update the scene
        scene->update();
        window.clear();
        window.setView(sceneView);

        //center view to player after running update()
        sceneView.setCenter(scene->player->getPosition());
        scene->draw(&window);

        //draw hud
        window.setView(hudView);
        hud.health = scene->player->health;
        hud.update();
        window.draw(hud);

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

	case sf::Keyboard::C:
        scene->drawColliders = !scene->drawColliders;
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
