#pragma once
#include <LDtkLoader/Entity.hpp>
#include "AnimatedGameObject.h"
#include "RigidBody.h"

class Player : public RigidBody
{
public:
	void init() override;
	void update() override;

	void onCollisionEnter(RigidBody* other) override;
};
