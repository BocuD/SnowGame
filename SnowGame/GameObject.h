#pragma once
#include <string>
#include <SFML/Graphics/Sprite.hpp>

//#include "Component.h"

class GameObject : public sf::Sprite
{
public:
	std::string name;

	virtual void init() {};
	virtual void update() {};
};