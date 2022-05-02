#include "Mob.h"

void Mob::init()
{
	textureSize = { 376, 256 };
	setOrigin(376 / 2, 225);
	setScale(0.08f, 0.08f);
	delayFrames = 3;
	addTexture("Assets/Sprites/Slimes/slimegreen.png", 30);
	setTextureId(0);
	animate = true;

	colliderSize = { 20, 15 };
}

void Mob::update()
{
	RigidBody::update();
}