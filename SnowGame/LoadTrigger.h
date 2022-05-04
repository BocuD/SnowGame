#pragma once
#include "Collider.h"

class LoadTrigger : public Collider
{
public:
	std::string levelName;
	sf::Vector2f spawnPos;
};
