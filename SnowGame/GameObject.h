#pragma once
#include <string>
#include <SFML/Graphics/Sprite.hpp>

class Scene;

class GameObject : public sf::Sprite
{
public:
	std::string name;
	Scene* scene;
	bool destroyed = false;

	virtual void init() {};
	virtual void update() {};

	virtual void destroy();
	virtual void onDestroy() {};
};