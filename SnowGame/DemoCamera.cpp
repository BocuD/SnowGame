#include "DemoCamera.h"

#include "VectorUtilities.h"

void DemoCamera::init()
{
	RigidBody::init();
}

void DemoCamera::loadEntityData(const ldtk::Entity& entity)
{
	for (auto element : entity.getArrayField<ldtk::IntPoint>("Waypoint"))
	{
		sf::Vector2f pos = { (float)element.value().x * 16, (float)element.value().y * 16 };
		positions.push_back(pos);
	}

	targetPosition = positions[0];

	RigidBody::loadEntityData(entity);
}

void DemoCamera::fixedUpdate()
{
	if(VectorUtilities::distance(positions[currentPosition], getPosition()) < 1)
	{
		currentPosition++;
		if(currentPosition > positions.size()-1)
		{
			currentPosition = 0;
		}

		targetPosition = positions[currentPosition];
	}

	targetVelocity = VectorUtilities::normalizeVector(targetPosition - getPosition()) * 20.f;
	velocity = VectorUtilities::lerp(velocity, targetVelocity, 0.02f);

	RigidBody::fixedUpdate();
}
