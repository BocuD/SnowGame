#pragma once
#include "RigidBody.h"

class Mob : public RigidBody
{
public:
	void init() override;
	void onCollisionEnter(RigidBody* other) override;
};
