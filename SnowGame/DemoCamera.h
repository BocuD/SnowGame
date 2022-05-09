#pragma once
#include "RigidBody.h"

class DemoCamera : public RigidBody
{
public:
	void init() override;
	void loadEntityData(const ldtk::Entity& entity) override;
	void fixedUpdate() override;
private:
	int currentPosition;
	sf::Vector2f targetPosition;
	sf::Vector2f targetVelocity;
	std::vector<sf::Vector2f> positions;
};
