#pragma once
#include <vector>
#include <SFML/Graphics/Rect.hpp>

#include "AnimatedGameObject.h"

class RigidBody : public AnimatedGameObject
{
public:
	void handleCollisions(std::vector<sf::FloatRect> colliders);
	virtual sf::FloatRect getColliderRect();

	bool grounded = true;
};
