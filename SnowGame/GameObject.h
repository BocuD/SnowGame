#pragma once
#include <string>
#include <SFML/Graphics/Sprite.hpp>

#include "Scene.h"

//#include "Component.h"

class GameObject : public sf::Sprite
{
public:
	std::string name;
	Scene* scene;

	virtual void init() {};
	virtual void update() {};

	virtual void destroy()
	{
		scene->removeEntity(this);
	}
};