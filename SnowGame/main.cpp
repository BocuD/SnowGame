#include <LDtkLoader/Project.hpp>
#include <SFML/Graphics.hpp>

#include "Scene.h"
#include "Fonts.h"
#include "Hud.h"
#include "Player.h"
#include "ParallaxBackground.h"

void handleInput(const sf::Event::KeyEvent& key);

std::vector<Scene*> scenes;
ParallaxBackground background;
Hud hud;

sf::Texture t0, t1, t2, t3, t4, t5;

void loadBGTextures()
{
	t0.loadFromFile("Assets/Backgrounds/Glacial-mountains/Layers/sky.png");
	t1.loadFromFile("Assets/Backgrounds/Glacial-mountains/Layers/clouds_bg.png");
	t2.loadFromFile("Assets/Backgrounds/Glacial-mountains/Layers/glacial_mountains.png");
	t3.loadFromFile("Assets/Backgrounds/Glacial-mountains/Layers/clouds_mg_3.png");
	t4.loadFromFile("Assets/Backgrounds/Glacial-mountains/Layers/clouds_mg_2.png");
	t5.loadFromFile("Assets/Backgrounds/Glacial-mountains/Layers/clouds_mg_1.png");

    t0.setRepeated(true);
    t1.setRepeated(true);
    t2.setRepeated(true);
    t3.setRepeated(true);
    t4.setRepeated(true);
    t5.setRepeated(true);

	background.sprites[0].setTexture(t0);
	background.sprites[1].setTexture(t1);
	background.sprites[2].setTexture(t2);
	background.sprites[3].setTexture(t3);
	background.sprites[4].setTexture(t4);
	background.sprites[5].setTexture(t5);
}

int main(char* args)
{
    sf::RenderWindow window(sf::VideoMode(1280, 720), "Snow Game");
    window.setVerticalSyncEnabled(true);

    ldtk::Project project;
    project.loadFromFile("Assets/world.ldtk");

    std::cout << "Successfully loaded project file " << project.getFilePath() << std::endl;

    //for (const ldtk::Level& level : project.getWorld().allLevels())
    //{
    //    Scene* scene = new Scene(&project, level.name);
    //    scenes.push_back(scene);
    //    //scenes.emplace_back(&project, level.name);
    //}

    Scene* scene = new Scene(&project, "World_Level_2");
    scenes.push_back(scene);

    hud.init();

    sf::Clock timer;
    timer.restart();

    Fonts::loadFonts();

    sf::View sceneView({ 0, 0, 1280, 720 });
    sf::View hudView({ 0, 0, 1280, 720 });
    sf::View backgroundView({ 0, 0, 1280, 720 });

    sceneView.zoom(0.3f);
	hudView.zoom(1.0f);
    backgroundView.zoom(1.0f);

    background.addLayer("Assets/Backgrounds/Glacial-mountains/Layers/sky.png");
    background.addLayer("Assets/Backgrounds/Glacial-mountains/Layers/clouds_bg.png");
    background.addLayer("Assets/Backgrounds/Glacial-mountains/Layers/glacial_mountains.png");
    background.addLayer("Assets/Backgrounds/Glacial-mountains/Layers/clouds_mg_3.png");
    background.addLayer("Assets/Backgrounds/Glacial-mountains/Layers/clouds_mg_2.png");
    background.addLayer("Assets/Backgrounds/Glacial-mountains/Layers/clouds_mg_1.png");

    loadBGTextures();

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
                backgroundView.setSize({ (float)e.size.width, (float)e.size.height });

                sceneView.zoom(720 / sceneView.getSize().y * 0.3f);
                hudView.zoom(720 / hudView.getSize().y);
                backgroundView.zoom(720 / backgroundView.getSize().y);

                hudView.setCenter(hudView.getSize().x / 2, hudView.getSize().y / 2);
                break;
            case sf::Event::KeyPressed:
                handleInput(e.key);
                break;
            }
        }

    	//update the scenes
        for (auto scene : scenes)
        {
            scene->update();
        }
        
        window.clear();

        //center view to player after running update()
        sceneView.setCenter(scenes[0]->player->getPosition());
        background.update(-sceneView.getCenter() / 10.f);

        //backgroundView.setCenter(sceneView.getCenter() / 2.f);
        window.setView(backgroundView);
        window.draw(background);

        window.setView(sceneView);

        //draw the scenes
        for (auto scene : scenes)
        {
            scene->draw(&window);
        }

        //draw hud
        window.setView(hudView);
        hud.health = scenes[0]->player->health;
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
		for (auto scene : scenes)
		{
            scene->drawColliders = !scene->drawColliders;
		}
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
