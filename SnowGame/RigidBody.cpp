#include "RigidBody.h"

#include <iostream>
#include <vector>
#include <SFML/Graphics/Rect.hpp>

#include "Collider.h"

void RigidBody::handleCollisions(std::vector<Collider> colliders, std::vector<RigidBody*> rigidBodies)
{
	// do collision checks
	updateColliderRect();

	for (Collider& col : colliders)
	{
		sf::FloatRect intersect;

		if (col.isTrigger) //handle trigger events
		{
			const bool wasInTrigger = std::find(intersectingTriggers.begin(), intersectingTriggers.end(), &col) != intersectingTriggers.end();

			if (colliderRect.intersects(col.bounds, intersect))
			{
				if (!wasInTrigger)
				{
					intersectingTriggers.push_back(&col);
					onTriggerEnter(&col);
				}
			}
			else
			{
				if (wasInTrigger)
				{
					intersectingTriggers.erase(std::remove(intersectingTriggers.begin(), intersectingTriggers.end(), &col), intersectingTriggers.end());
					onTriggerExit(&col);
				}
			}
		}
		else //resolve collisions
		{
			if (colliderRect.intersects(col.bounds, intersect))
			{
				if (intersect.width < intersect.height)
				{
					//handle wall collisions
					if (colliderRect.left < intersect.left)
						move(-intersect.width, 0);
					else
						move(intersect.width, 0);
				}
				else
				{
					//touching the floor, we need to move up: we're grounded
					if (colliderRect.top < intersect.top)
					{
						move(0, -intersect.height);
						grounded = true;
					}
					//touching the ceiling, we need to move downwards
					else
					{
						move(0, intersect.height);
					}
				}
			}
		}
	}

	for (const auto rb : rigidBodies)
	{
		if (rb == this) continue;

		sf::FloatRect intersect;
		const bool wasColliding = std::find(intersectingBodies.begin(), intersectingBodies.end(), rb) != intersectingBodies.end();

		if(colliderRect.intersects(rb->colliderRect, intersect))
		{
			if (!wasColliding) 
			{
				intersectingBodies.push_back(rb);
				onCollisionEnter(rb);
			}
		}
		else
		{
			if (wasColliding) 
			{
				intersectingBodies.erase(std::remove(intersectingBodies.begin(), intersectingBodies.end(), rb), intersectingBodies.end());
				onCollisionExit(rb);
			}
		}
	}
}

void RigidBody::updateColliderRect()
{
	const sf::Vector2f position = getPosition();
	colliderRect = { position.x - colliderSize.x / 2, position.y - colliderSize.y, colliderSize.x, colliderSize.y };
}

void RigidBody::destroy()
{
	scene->rigidBodies.erase(std::remove(scene->rigidBodies.begin(), scene->rigidBodies.end(), this), scene->rigidBodies.end());
	AnimatedGameObject::destroy();
}
