#include "Game.h"

#include <future>
#include <LDtkLoader/Project.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/Window/Event.hpp>

#include "Fonts.h"
#include "Hud.h"
#include "ParallaxBackground.h"
#include "Scene.h"

ldtk::Project* project;
std::vector<Scene*> scenes;
Scene* activeScene;

ParallaxBackground background;
Hud hud;

sf::Texture t0, t1, t2, t3, t4, t5;

sf::View* Game::sceneView;
sf::View* Game::backgroundView;
sf::View* Game::hudView;

void handleInput(const sf::Event::KeyEvent& key);

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

void Game::resetViews()
{
    sf::FloatRect sceneRect = activeScene->sceneRect;

    sceneView->zoom(720 / sceneView->getSize().y * 0.3f);
    hudView->zoom(720 / hudView->getSize().y * 0.8f);
    backgroundView->zoom(720 / backgroundView->getSize().y);

    hudView->setCenter(hudView->getSize().x / 2, hudView->getSize().y / 2);

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

	project = new ldtk::Project;
    project->loadFromFile("Assets/world.ldtk");

    //force cache all tilemaps
    for (auto tileset : project->allTilesets())
    {
        TextureManager::getTexture("Assets/" + tileset.path);
    }

    std::cout << "Successfully loaded project file " << project->getFilePath() << std::endl;

    sceneView = new sf::View({ 0, 0, 1280, 720 });
    hudView = new sf::View({ 0, 0, 1280, 720 });
    backgroundView = new sf::View({ 0, 0, 1280, 720 });

    Scene* scene = new Scene(project, "World_Level_0");
    addScene(scene);

    setActiveScene(scene);

    hud.init();

    Fonts::loadFonts();

    background.addLayer("Assets/Backgrounds/Glacial-mountains/Layers/sky.png");
    background.addLayer("Assets/Backgrounds/Glacial-mountains/Layers/clouds_bg.png");
    background.addLayer("Assets/Backgrounds/Glacial-mountains/Layers/glacial_mountains.png");
    background.addLayer("Assets/Backgrounds/Glacial-mountains/Layers/clouds_mg_3.png");
    background.addLayer("Assets/Backgrounds/Glacial-mountains/Layers/clouds_mg_2.png");
    background.addLayer("Assets/Backgrounds/Glacial-mountains/Layers/clouds_mg_1.png");

    background.offset.y = -250;

    loadBGTextures();
}

void Game::update(float dt)
{
    //update the scene
	for (auto scene : scenes)
	{
        scene->update(dt);
	}
}

void Game::fixedUpdate() 
{
    //run fixedUpdate in all scenes
    for (auto scene : scenes)
    {
        scene->fixedUpdate();
    }
}

void Game::addScene(Scene* newScene)
{
    scenes.push_back(newScene);
}

void Game::loadScene(std::string name, std::function<void(Scene* scene)> callback)
{
    Scene* scene = new Scene(Game::getProject(), name);
    std::cout << "adding scene" << std::endl;
    Game::addScene(scene);
    callback(scene);
}

void Game::loadSceneAsync(std::string levelName, std::function<void(Scene* scene)> callback)
{
    std::thread* test = new std::thread(loadScene, levelName, callback);
}

void Game::removeScene(Scene* scene)
{
    scenes.erase(std::remove(scenes.begin(), scenes.end(), scene), scenes.end());
    delete scene;
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

void Game::draw(sf::RenderWindow* window)
{
    //center view to player after running update()
    sceneView->setCenter(activeScene->player->getPosition());
    sf::FloatRect sceneRect = activeScene->sceneRect;

    sf::Vector2f center = sceneView->getCenter();
    sf::Vector2f size = sceneView->getSize();

    sf::FloatRect viewRect = { center.x - size.x / 2, center.y - size.y / 2, size.x, size.y };

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
    for (auto scene : scenes)
    {
        scene->draw(window);
    }

    //draw hud
    window->setView(*hudView);
    hud.health = activeScene->player->health;
    hud.update();
    window->draw(hud);
}

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
        handleInput(e.key);
        break;
    }
}

//can be used for gui navigation
void handleInput(const sf::Event::KeyEvent& key)
{
    switch (key.code)
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