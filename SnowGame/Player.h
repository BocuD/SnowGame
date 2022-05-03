#pragma once
#include <LDtkLoader/Entity.hpp>
#include "AnimatedGameObject.h"
#include "RigidBody.h"

class Player : public RigidBody
{
public:
	void init() override;
	void runStateMachine(bool moving);
	void update() override;

	void addHealth(int amount);
	void removeHealth(int amount);
	void onCollisionEnter(RigidBody* other) override;

	float health;
	const float maxHealth = 25;
};
