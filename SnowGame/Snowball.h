#pragma once
#include "RigidBody.h"

class Snowball : public RigidBody
{
public:
	void init() override;
	void update() override;

	void onCollisionEnter(RigidBody* other) override;
};
