#pragma once
#include <future>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/VideoMode.hpp>

#include "LDtkLoader/Project.hpp"
#include "Scene.h"
#include "SFXManager.h"
#include "TextureManager.h"

class Game
{
public:
	static void resetViews();
	void init();
	void update(float dt);
	void fixedUpdate();
	static void startGame();
	static void endGame();
	void draw(sf::RenderWindow* window);
	void eventHandler(const sf::Event e, sf::Window* window);

	static std::vector<std::unique_ptr<Scene>> scenes;

	static void loadScene(std::string name, std::function<void(Scene* scene)> callback);
	static void loadSceneAsync(std::string levelName, std::function<void(Scene* scene)> callback);
	static void removeScene(Scene* scene);

	static Scene* getActiveScene();
	static void setActiveScene(Scene* scene);

	static ldtk::Project* getProject();

	static bool hasFocus;
	static bool paused;

	static void setVSync(bool value);
	static void setFullScreen(bool value);

	static bool vsync;
	static bool fullScreen;
	static bool graphicsChanges;

private:
	TextureManager textureManager;
	SFXManager sfxManager;

	static bool inMenu;

	static sf::View* sceneView;
	static sf::View* backgroundView;
	static sf::View* hudView;
	static sf::View* menuView;

	static Scene* activeScene;
};
