#pragma once
#include "RigidBody.h"

class Mob : public RigidBody
{
public:
	void init() override;
	void update(float dt) override;
	void onCollisionEnter(RigidBody* other) override;
	void loadEntityData(const ldtk::Entity& entity) override;
	sf::Vector2f* patrolPoints;
private:
	float targetVelocity = 20;
	int currentPoint = 0;
};
