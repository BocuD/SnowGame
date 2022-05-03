#pragma once
#include <string>
#include <SFML/Graphics/Rect.hpp>

class Collider
{
public:
	bool isTrigger = false;
	sf::FloatRect bounds;
	std::string tag;
};
