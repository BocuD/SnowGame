#pragma once
#include <memory>
#include <string>
#include <vector>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Vector2.hpp>

//#include "Component.h"

class GameObject : public sf::Sprite
{
public:
	std::string name;
	//std::vector<std::unique_ptr<Component>> components;

	virtual void init();
	virtual void update();
	virtual bool checkOverlap(GameObject* other);
};

enum EntityType
{
	tile,
	player,
	other,
};