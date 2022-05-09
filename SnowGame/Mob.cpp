#include "Mob.h"

#include "TextureManager.h"
#include "VectorUtilities.h"

void Mob::init()
{
	textureSize = { 376, 256 };
	setOrigin({ 376 / 2, 200 });
	setScale({0.08f, 0.08f});
	delayFrames = 3;
	addTexture(TextureManager::getTexture("Assets/Sprites/Slimes/slimegreen.png"), 30);
	setTextureId(0);
	animate = true;

	colliderSize = { 17, 13 };
}

void Mob::update(float dt)
{

	if(VectorUtilities::vectorLength(velocity) < 1)
	{
		targetVelocity *= -1;
		velocity.x = targetVelocity;
	}
	RigidBody::update(dt);
}

void Mob::onCollisionEnter(RigidBody* other)
{
	if(other->name == "Snowball")
	{
		destroy();
	}
}

void Mob::loadEntityData(const ldtk::Entity& entity)
{
	auto& pointsField = entity.getArrayField<ldtk::IntPoint>("Patrol");

	patrolPoints = new sf::Vector2f[pointsField.size()];
	for (size_t i = 0; i < pointsField.size(); i++)
	{
		patrolPoints[i] = { (float)pointsField[i].value().x * 16, (float)pointsField[i].value().y * 16 };
	}

	RigidBody::loadEntityData(entity);
}
