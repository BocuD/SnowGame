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

	void onTriggerEnter(Collider* other) override;
	void onTriggerExit(Collider* other) override;

	float health;
	const float maxHealth = 25;

	bool upWasPressed = false;
	bool shiftWasPressed = false;
	bool spaceWasPressed = false;
	int disableStateMachine = 0;
	int jumpCount = 3;
	int invincibilityFrames = 0;
	int frameMod;
	int onLadder = 0;
};
