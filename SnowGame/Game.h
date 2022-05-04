#pragma once
#include <future>
#include <SFML/Graphics/RenderWindow.hpp>

#include "LDtkLoader/Project.hpp"
#include "Scene.h"
#include "TextureManager.h"

class Game
{
public:
	void init();
	void update();
	void draw(sf::RenderWindow* window);
	void eventHandler(const sf::Event e);

	static void addScene(Scene* scene);
	static void loadScene(std::string name, std::function<void(Scene* scene)> callback);
	static void loadSceneAsync(std::string levelName, std::function<void(Scene* scene)> callback);
	static void removeScene(Scene* scene);

	static Scene* getActiveScene();
	static void setActiveScene(Scene* scene);

	static ldtk::Project* getProject();
private:
	TextureManager textureManager;
};
