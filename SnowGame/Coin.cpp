#include "Coin.h"

#include "Game.h"
#include "TextureManager.h"

void Coin::init()
{
	textureSize = { 16, 16 };
	delayFrames = 5;
	addTexture(TextureManager::getTexture("Assets/Coin_Gems/MonedaD.png"), 5);
	setTextureId(0);
	setOrigin({ 8, 3 });
	colliderSize = { 12, 12 };
	animate = true;

	AnimatedGameObject::init();
}

void Coin::fixedUpdate()
{
	if(pickupFrame > 0)
	{
		uint8_t a = pickupFrame * 25;
		setColor({ 255, 255, 255, a });
		pickupFrame--;
		move({ 0, -0.6f });
		if(pickupFrame == 0)
		{
			destroy();
		}
	}

	RigidBody::fixedUpdate();
}

void Coin::pickup()
{
	pickupFrame = 10;
}

void Gem::init()
{
	textureSize = { 16, 16 };
	delayFrames = 8;
	addTexture(TextureManager::getTexture("Assets/Coin_Gems/spr_coin_strip4.png"), 4);
	setTextureId(0);
	setOrigin({ 8, 3 });
	colliderSize = { 12, 12 };
	animate = true;

	AnimatedGameObject::init();
}

void Gem::loadEntityData(const ldtk::Entity& entity)
{
	index = entity.getField<int>("index").value();

	Coin::loadEntityData(entity);
}
