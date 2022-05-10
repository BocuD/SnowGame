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

ParallaxBackground backgroundSnow;
ParallaxBackground backgroundCave;
ParallaxBackground* currentBackground;
Hud hud;

Menu* mainMenu;
Menu* pauseMenu;
Menu* optionsMenu;
Menu* levelTransition;

Menu* currentMenu;

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

    auto scene = std::make_unique<Scene>(project, "World_Level_0");
    scene->disableHud = true;
    scenes.push_back(std::move(scene));
	setActiveScene(scenes.back().get());

    hud.init();

    //create main menu
    mainMenu = new Menu();
    mainMenu->setTitle("Snowball Jump", 60);
    mainMenu->addOption("Start game", []
    {
        startGame();
    });
    mainMenu->addOption("Options", []
    {
        currentMenu = optionsMenu;
    });
    mainMenu->addOption("Credits", [] {});

    currentMenu = mainMenu;

    //create options menu
    optionsMenu = new Menu();
    optionsMenu->setTitle("Options", 45);
    optionsMenu->addOption("Toggle fullscreen", []
    {
        setFullScreen(!fullScreen);
    });
    optionsMenu->addOption("Toggle vsync", [] 
    {
        setVSync(!vsync);
    });
    optionsMenu->addOption("Return to main menu", []
    {
        currentMenu = mainMenu;
    });

    //create pause menu
    pauseMenu = new Menu();
    pauseMenu->setTitle("Game Paused", 45);
    pauseMenu->addOption("Continue", []
    {
        paused = false;
        currentMenu = nullptr;
        activeScene->disableHud = false;
    });
    pauseMenu->addOption("Return to main menu", []
    {
        endGame();
    });

    //create level transition menu
    levelTransition = new Menu();
    levelTransition->setTitle("Level 1", 45);

    //todo: turn these into an object in the scene
    backgroundSnow.addLayer("Assets/Backgrounds/Glacial-mountains/Layers/sky.png");
    backgroundSnow.addLayer("Assets/Backgrounds/Glacial-mountains/Layers/clouds_bg.png");
    backgroundSnow.addLayer("Assets/Backgrounds/Glacial-mountains/Layers/glacial_mountains.png");
    backgroundSnow.addLayer("Assets/Backgrounds/Glacial-mountains/Layers/clouds_mg_3.png");
    backgroundSnow.addLayer("Assets/Backgrounds/Glacial-mountains/Layers/clouds_mg_2.png");
    backgroundSnow.addLayer("Assets/Backgrounds/Glacial-mountains/Layers/clouds_mg_1.png");

    backgroundSnow.offset.x = -1000;
    backgroundSnow.offset.y = -500;

    backgroundCave.addLayer("Assets/Backgrounds/pixel-art-cave/red_L1.png");
    backgroundCave.addLayer("Assets/Backgrounds/pixel-art-cave/red_L2.png");
    backgroundCave.addLayer("Assets/Backgrounds/pixel-art-cave/red_L3.png");
    backgroundCave.addLayer("Assets/Backgrounds/pixel-art-cave/red_L4.png");

    backgroundCave.offset.x = -1000;
    backgroundCave.offset.y = -500;

    currentBackground = &backgroundSnow;
}

void Game::update(float dt)
{
    if (paused) return;

    //update the scene
	for (auto& scene : scenes)
	{
        if (scene->enabled)
            scene->update(dt);
	}

    //remove destroyed scenes
    for (size_t i = 0; i < scenes.size(); i++)
    {
        Scene* scene = scenes[i].get();
        if (scene->destroyed)
        {
            removeScene(scene);
        }
    }
}

void Game::fixedUpdate() 
{
    if (paused) return;

    //run fixedUpdate in all scenes
    for (auto& scene : scenes)
    {
        if (scene->enabled)
            scene->fixedUpdate();
    }
}

void Game::draw(sf::RenderWindow* window)
{
    if (activeScene != nullptr) 
    {
        //center view to player after running update()
        if (activeScene->player != nullptr)
            sceneView->setCenter(activeScene->player->getPosition());

        sf::FloatRect sceneRect = activeScene->sceneRect;

        sf::Vector2f center = sceneView->getCenter();
        sf::Vector2f size = sceneView->getSize();

        sf::FloatRect viewRect = { { center.x - size.x / 2, center.y - size.y / 2}, {size.x, size.y } };

        if (viewRect.left < sceneRect.left) viewRect.left = sceneRect.left;
        if (viewRect.left + viewRect.width > sceneRect.left + sceneRect.width) viewRect.left = sceneRect.left + sceneRect.width - viewRect.width;

        if (viewRect.top < sceneRect.top) viewRect.top = sceneRect.top;
        if (viewRect.top + viewRect.height > sceneRect.top + sceneRect.height) viewRect.top = sceneRect.top + sceneRect.height - viewRect.height;

        sceneView->reset(viewRect);

        currentBackground->update(-sceneView->getCenter() / 10.f);

        window->setView(*backgroundView);
        window->draw(*currentBackground);
        window->setView(*sceneView);
    }

    //draw all scenes
    for (auto& scene : scenes)
    {
        if (scene->enabled)
            scene->draw(window);
    }

    if (activeScene != nullptr) 
    {
        if (!activeScene->disableHud)
        {
            //draw hud
            window->setView(*hudView);
            hud.health = ((Player*)activeScene->player)->health;
            hud.update();
            window->draw(hud);
        }
    }

    //draw menu
    if (currentMenu != nullptr)
    {
        window->setView(*menuView);
        window->draw(*currentMenu);
    }
}

void Game::startGame()
{
    project = new ldtk::Project;
    project->loadFromFile("Assets/world.ldtk");

    Scene* menuScene = getActiveScene();

    loadScene("World_Level_0", [](Scene* newScene)
    {
        setActiveScene(newScene);
    });

    currentMenu = levelTransition;
    inMenu = false;

    removeScene(menuScene);
}

void Game::endGame()
{
	for (auto& scene : scenes)
	{
        scene->destroy();
	}

    project = new ldtk::Project;
    project->loadFromFile("Assets/menu.ldtk");

    Scene* gameScene = getActiveScene();

    loadScene("World_Level_0", [](Scene* newScene)
    {
        setActiveScene(newScene);
        newScene->disableHud = true;
    });

    currentMenu = mainMenu;
    inMenu = true;
    paused = false;
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
    if (activeScene == scene) activeScene = nullptr;

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

void Game::setVSync(bool value)
{
    vsync = value;
    graphicsChanges = true;
}

void Game::setFullScreen(bool value)
{
    fullScreen = value;
    graphicsChanges = true;
}

bool Game::inMenu = true;
bool Game::paused = false;

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

        if (e.key.code == sf::Keyboard::Key::Escape)
        {
	        if(!Game::inMenu)
	        {
                if (currentMenu == nullptr)
                {
                    paused = true;
                    currentMenu = pauseMenu;
                    activeScene->disableHud = true;
                }
                else
                {
                    paused = false;
                    currentMenu = nullptr;
                    activeScene->disableHud = false;
                }
	        }
        }
        
        if (currentMenu != nullptr)
            currentMenu->handleInput(e.key);
        break;
    }
}

bool Game::vsync = true;
bool Game::fullScreen = false;
bool Game::graphicsChanges = false;