#pragma once
#include <future>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/VideoMode.hpp>

#include "LDtkLoader/Project.hpp"
#include "Scene.h"
#include "TextureManager.h"

class Game
{
public:
	static void resetViews();
	void init();
	void update(float dt);
	void fixedUpdate();
	static void startGame();
	void draw(sf::RenderWindow* window);
	void eventHandler(const sf::Event e, sf::Window* window);

	static void loadScene(std::string name, std::function<void(Scene* scene)> callback);
	static void loadSceneAsync(std::string levelName, std::function<void(Scene* scene)> callback);
	static void removeScene(Scene* scene);

	static Scene* getActiveScene();
	static void setActiveScene(Scene* scene);

	static ldtk::Project* getProject();

	static bool hasFocus;

private:
	TextureManager textureManager;

	static sf::View* sceneView;
	static sf::View* backgroundView;
	static sf::View* hudView;
	static sf::View* menuView;

	static std::vector<std::unique_ptr<Scene>> scenes;
	static Scene* activeScene;
};
