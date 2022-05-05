#include "Coin.h"

#include "TextureManager.h"

void Coin::init()
{
	textureSize = { 16, 16 };
	delayFrames = 5;
	addTexture(TextureManager::getTexture("Assets/Coin_Gems/MonedaD.png"), 5);
	setTextureId(0);
	setOrigin(8, 3);
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
		move(0, -0.6f);
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
