#pragma once
#include "RigidBody.h"

class Snowball : public RigidBody
{
public:
	void init() override;
	void update() override;
	int num = 120;
};
