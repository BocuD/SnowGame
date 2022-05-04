#include "Mob.h"

#include <SFML/Graphics/Texture.hpp>

#include "TextureManager.h"

void Mob::init()
{
	textureSize = { 376, 256 };
	setOrigin(376 / 2, 200);
	setScale(0.08f, 0.08f);
	delayFrames = 3;
	addTexture(TextureManager::getTexture("Assets/Sprites/Slimes/slimegreen.png"), 30);
	setTextureId(0);
	animate = true;

	colliderSize = { 17, 13 };
}

void Mob::update()
{
	RigidBody::update();
}

void Mob::onCollisionEnter(RigidBody* other)
{
	if(other->name == "Snowball")
	{
		destroy();
	}
}
