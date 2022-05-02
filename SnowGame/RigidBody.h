#pragma once
#include <vector>
#include <SFML/Graphics/Rect.hpp>

#include "AnimatedGameObject.h"

class RigidBody : public AnimatedGameObject
{
public:
	void handleCollisions(std::vector<sf::FloatRect> colliders, std::vector<RigidBody*> rigidBodies);
	void updateColliderRect();

	sf::Vector2f colliderSize;
	sf::FloatRect colliderRect;
	std::vector<RigidBody*> intersectingBodies;

	bool grounded = true;

	virtual void onCollisionEnter(RigidBody* other) {};
	virtual void onCollisionExit(RigidBody* other) {};
};
