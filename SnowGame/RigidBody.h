#pragma once
#include <vector>
#include <vector>
#include <SFML/Graphics/Rect.hpp>

#include "AnimatedGameObject.h"
#include "Collider.h"

class RigidBody : public AnimatedGameObject
{
public:
	void handleCollisions(std::vector<Collider> colliders, std::vector<RigidBody*> rigidBodies);
	void updateColliderRect();

	sf::Vector2f colliderSize;
	sf::FloatRect colliderRect;

	std::vector<RigidBody*> intersectingBodies;
	std::vector<Collider*> intersectingTriggers;

	bool grounded = true;

	virtual void onCollisionEnter(RigidBody* other) {};
	virtual void onCollisionExit(RigidBody* other) {};

	virtual void onTriggerEnter(Collider* other) {};
	virtual void onTriggerExit(Collider* other) {};
};
