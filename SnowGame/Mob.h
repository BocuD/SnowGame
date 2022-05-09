#pragma once
#include "RigidBody.h"

class Mob : public RigidBody
{
public:
	void init() override;
	void update(float dt) override;
	void onCollisionEnter(RigidBody* other) override;
	void loadEntityData(const ldtk::Entity& entity) override;
	
private:
	float targetVelocity = 20;

	bool patrol = false;
	sf::Vector2f* patrolPoints;
	int currentPoint = 0;
};
