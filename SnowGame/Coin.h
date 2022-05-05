#pragma once
#include "RigidBody.h"

class Coin : public RigidBody
{
public:
	void init() override;
	void fixedUpdate() override;
	void pickup();
private:
	int pickupFrame = 0
	;
};
