#pragma once
#include "RigidBody.h"

class Mob : public RigidBody
{
public:
	void init() override;
	void update(float dt) override;
	void onCollisionEnter(RigidBody* other) override;
private:
	float targetVelocity = 20;
	int currentPoint = 0;
	sf::Vector2f patrol[];
};
