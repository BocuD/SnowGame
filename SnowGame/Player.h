#pragma once

#include "RigidBody.h"

class Player : public RigidBody
{
public:
	void init() override;
	void update(float dt) override;
	void fixedUpdate() override;

	void addHealth(int amount);
	void removeHealth(int amount);
	void onCollisionEnter(RigidBody* other) override;

	void onTriggerEnter(Collider* other) override;
	void onTriggerExit(Collider* other) override;

	int health;
	const int maxHealth = 25;

	bool upWasPressed = false;
	bool shiftWasPressed = false;
	bool spaceWasPressed = false;
	bool moving = false;
	int disableStateMachine = 0;
	int jumpCount = 3;
	int invincibilityFrames = 0;
	int frameMod;
	int onLadder = 0;
};
