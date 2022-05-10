#pragma once
#include <SFML/Audio/Sound.hpp>

#include "RigidBody.h"

class Mob : public RigidBody
{
public:
	void init() override;
	void update(float dt) override;
	void fixedUpdate() override;
	void onCollisionEnter(RigidBody* other) override;
	void loadEntityData(const ldtk::Entity& entity) override;
	
private:
	float targetVelocity = 20;
	sf::Sound die;
	bool patrol = false;
	sf::Vector2f* patrolPoints;
	int currentPoint = 0;
	int dieCounter;
};
