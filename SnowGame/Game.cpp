#include "Game.h"

#include <future>
#include <LDtkLoader/Project.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/Window/Event.hpp>

#include "Fonts.h"
#include "Hud.h"
#include "Menu.h"
#include "ParallaxBackground.h"
#include "Player.h"
#include "Scene.h"

ldtk::Project* project;

ParallaxBackground background;
Hud hud;
Menu* menu;

sf::View* Game::sceneView;
sf::View* Game::backgroundView;
sf::View* Game::hudView;
sf::View* Game::menuView;

std::vector<std::unique_ptr<Scene>> Game::scenes;
Scene* Game::activeScene;

void Game::resetViews()
{
    sf::FloatRect sceneRect = activeScene->sceneRect;

    sceneView->zoom(720 / sceneView->getSize().y * 0.3f);
    hudView->zoom(720 / hudView->getSize().y * 0.8f);
    backgroundView->zoom(720 / backgroundView->getSize().y);
    menuView->zoom(720 / menuView->getSize().y);
    
    hudView->setCenter({ hudView->getSize().x / 2, hudView->getSize().y / 2 });
    menuView->setCenter({ 0, 0 });

    if (sceneRect.height < sceneView->getSize().y)
    {
        float diff = sceneRect.height / sceneView->getSize().y;
        sceneView->setSize(diff * sceneView->getSize());
        backgroundView->setSize(diff * backgroundView->getSize());
    }

    if (sceneRect.width < sceneView->getSize().x)
    {
        float diff = sceneRect.width / sceneView->getSize().x;
        sceneView->setSize(diff * sceneView->getSize());
        backgroundView->setSize(diff * backgroundView->getSize());
    }
}

void Game::init()
{
    hasFocus = true;
    
    //load project
	project = new ldtk::Project;
    project->loadFromFile("Assets/menu.ldtk");

    std::cout << "Successfully loaded project file " << project->getFilePath() << std::endl;

    //force cache all tilemaps
    for (auto tileset : project->allTilesets())
    {
        TextureManager::getTexture("Assets/" + tileset.path);
    }

    sceneView = new sf::View({ { 0, 0}, {1280, 720 } });
    hudView = new sf::View({ { 0, 0}, {1280, 720 } });
    backgroundView = new sf::View({ { 0, 0}, {1280, 720 } });
    menuView = new sf::View({ { 0, 0}, {1280, 720 } });

    Fonts::loadFonts();

    auto scene = std::make_unique<Scene>(project, "World_Level_9");
    scene->disableHud = true;
    scenes.push_back(std::move(scene));
	setActiveScene(scenes.back().get());

    hud.init();

    menu = new Menu();

    //todo: turn these into an object in the scene
    background.addLayer("Assets/Backgrounds/Glacial-mountains/Layers/sky.png");
    background.addLayer("Assets/Backgrounds/Glacial-mountains/Layers/clouds_bg.png");
    background.addLayer("Assets/Backgrounds/Glacial-mountains/Layers/glacial_mountains.png");
    background.addLayer("Assets/Backgrounds/Glacial-mountains/Layers/clouds_mg_3.png");
    background.addLayer("Assets/Backgrounds/Glacial-mountains/Layers/clouds_mg_2.png");
    background.addLayer("Assets/Backgrounds/Glacial-mountains/Layers/clouds_mg_1.png");

    background.offset.x = -1000;
    background.offset.y = -500;
}

void Game::update(float dt)
{
    //update the scene
	for (auto& scene : scenes)
	{
        scene->update(dt);
	}
}

void Game::fixedUpdate() 
{
    //run fixedUpdate in all scenes
    for (auto& scene : scenes)
    {
        scene->fixedUpdate();
    }
}

void Game::draw(sf::RenderWindow* window)
{
    //center view to player after running update()
    if (activeScene->player != nullptr)
        sceneView->setCenter(activeScene->player->getPosition());

    sf::FloatRect sceneRect = activeScene->sceneRect;

    sf::Vector2f center = sceneView->getCenter();
    sf::Vector2f size = sceneView->getSize();

    sf::FloatRect viewRect = { { center.x - size.x / 2, center.y - size.y / 2}, {size.x, size.y }};

    if (viewRect.left < sceneRect.left) viewRect.left = sceneRect.left;
    if (viewRect.left + viewRect.width > sceneRect.left + sceneRect.width) viewRect.left = sceneRect.left + sceneRect.width - viewRect.width;

    if (viewRect.top < sceneRect.top) viewRect.top = sceneRect.top;
    if (viewRect.top + viewRect.height > sceneRect.top + sceneRect.height) viewRect.top = sceneRect.top + sceneRect.height - viewRect.height;

    sceneView->reset(viewRect);

    background.update(-sceneView->getCenter() / 10.f);

    window->setView(*backgroundView);
    window->draw(background);
    window->setView(*sceneView);

    //draw all scenes
    for (auto& scene : scenes)
    {
        scene->draw(window);
    }

    if (!activeScene->disableHud)
    {
        //draw hud
        window->setView(*hudView);
        hud.health = ((Player*)activeScene->player)->health;
        hud.update();
        window->draw(hud);
    }
    else
    {
        //draw menu
        window->setView(*menuView);
        menu->draw(window);
    }
}

void Game::startGame()
{
    project = new ldtk::Project;
    project->loadFromFile("Assets/world.ldtk");

    Scene* menuScene = getActiveScene();

    loadScene("World_Level_1", [](Scene* newScene)
    {
        setActiveScene(newScene);
    });

    removeScene(menuScene);
}

void Game::loadScene(std::string name, std::function<void(Scene* scene)> callback)
{
    std::unique_ptr<Scene> newScene = std::make_unique<Scene>(Game::getProject(), name);
    std::cout << "loaded scene" << std::endl;
    scenes.push_back(std::move(newScene));
    callback(scenes.back().get());
}

void Game::loadSceneAsync(std::string levelName, std::function<void(Scene* scene)> callback)
{
    std::thread* test = new std::thread(loadScene, levelName, callback);
}

void Game::removeScene(Scene* scene)
{
    for (size_t i = 0; i < scenes.size(); i++)
    {
	    if(scenes[i].get() == scene)
	    {
            scenes.erase(scenes.begin() + i);
            return;
	    }
    }
}

Scene* Game::getActiveScene()
{
    return activeScene;
}

void Game::setActiveScene(Scene* scene)
{
    activeScene = scene;

    resetViews();
}

ldtk::Project* Game::getProject()
{
    return project;
}

bool Game::hasFocus;

void Game::eventHandler(const sf::Event e, sf::Window* window)
{
    switch (e.type) {
    case sf::Event::EventType::Resized:
        sceneView->setSize({ (float)e.size.width, (float)e.size.height });
        hudView->setSize({ (float)e.size.width, (float)e.size.height });
        backgroundView->setSize({ (float)e.size.width, (float)e.size.height });
        resetViews();
        break;

    case sf::Event::EventType::GainedFocus:
        hasFocus = true;
        window->setFramerateLimit(0);
        break;

    case sf::Event::EventType::LostFocus:
        hasFocus = false;
        window->setFramerateLimit(30);
        break;

    case sf::Event::KeyPressed:
        if (e.key.code == sf::Keyboard::Key::C)
        {
	        for (auto& element : scenes)
	        {
                element->drawColliders = !element->drawColliders;
	        }
        }
        menu->handleInput(e.key);
        break;
    }
}