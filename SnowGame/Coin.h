#pragma once
#include "RigidBody.h"

class Coin : public RigidBody
{
public:
	void init() override;
	void initGem();
	void fixedUpdate() override;
	void pickup();
private:
	int pickupFrame = 0
	;
};

class Gem : public Coin
{
public:
	int index = 0;
	void init() override;
	void loadEntityData(const ldtk::Entity& entity) override;
};
