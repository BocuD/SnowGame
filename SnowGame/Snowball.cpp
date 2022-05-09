#include "Snowball.h"

#include "TextureManager.h"

void Snowball::init()
{
	textureSize = { 16, 16 };
	setOrigin({ 8, 8 });
	delayFrames = 3;
	addTexture(TextureManager::getTexture("Assets/Sprites/snowball.png"), 1);
	setTextureId(0);
	animate = true;
	
	colliderSize = { 9, 9 };
	colliderOffset = { 0, 4.5f };
}

void Snowball::fixedUpdate()
{
	velocity.y *= 0.93f;
	velocity.y += 14;

	if (grounded)
	{
		velocity.x *= 0.92f;
		rotate(sf::degrees(velocity.x * 0.3f));
	}

	RigidBody::fixedUpdate();

	num--;
	if (num == 0) destroy();
}