#pragma once
#include <vector>
#include <vector>
#include <SFML/Graphics/Rect.hpp>

#include "AnimatedGameObject.h"
#include "Collider.h"

class RigidBody : public AnimatedGameObject
{
public:
	void physicsTick(std::vector<Collider*> colliders, std::vector<RigidBody*> rigidBodies, float dt);
	void updateColliderRect();

	int ignorePhysics = 0;

	sf::Vector2f velocity;

	sf::Vector2f colliderSize;
	sf::FloatRect colliderRect;

	std::vector<RigidBody*> intersectingBodies;
	std::vector<Collider*> intersectingTriggers;

	bool grounded = false;
	sf::Vector2f colliderOffset;

	virtual void onCollisionEnter(RigidBody* other) {};
	virtual void onCollisionExit(RigidBody* other) {};

	virtual void onTriggerEnter(Collider* other) {};
	virtual void onTriggerExit(Collider* other) {};

private:
	sf::Vector2f lastPosition;
};
