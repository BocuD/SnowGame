#pragma once
#include "RigidBody.h"

class Snowball : public RigidBody
{
public:
	void init() override;
	void fixedUpdate() override;
	int num = 120;
};
