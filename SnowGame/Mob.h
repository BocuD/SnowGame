﻿#pragma once
#include "RigidBody.h"

class Mob : public RigidBody
{
public:
	void init() override;
	void update() override;

	void onCollisionEnter(RigidBody* other) override;
};
