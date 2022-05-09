#include "RigidBody.h"

#include <iostream>
#include <vector>
#include <SFML/Graphics/Rect.hpp>

#include "Collider.h"
#include "Scene.h"
#include "VectorUtilities.h"

using ::VectorUtilities;

void RigidBody::physicsTick(std::vector<Collider*> colliders, std::vector<RigidBody*> rigidBodies, float dt)
{
	if(ignorePhysics > 0)
	{
		ignorePhysics--;
		return;
	}

	//save temp position
	lastPosition = getPosition();

	//apply velocity
	move(velocity * dt);

	//update collider rect
	updateColliderRect();

	//handle collisions
	for (auto col : colliders)
	{
		if (col->isTrigger) //handle trigger events
		{
			const bool wasInTrigger = std::find(intersectingTriggers.begin(), intersectingTriggers.end(), col) != intersectingTriggers.end();

			if (colliderRect.findIntersection(col->bounds))
			{
				if (!wasInTrigger)
				{
					intersectingTriggers.push_back(col);
					onTriggerEnter(col);
				}
			}
			else
			{
				if (wasInTrigger)
				{
					intersectingTriggers.erase(std::remove(intersectingTriggers.begin(), intersectingTriggers.end(), col), intersectingTriggers.end());
					onTriggerExit(col);
				}
			}
		}
		else //resolve collisions
		{
			if (auto intersect = colliderRect.findIntersection(col->bounds))
			{
				sf::FloatRect rect = intersect.value();
				if (rect.width < rect.height)
				{
					//handle wall collisions
					if (colliderRect.left < rect.left)
						move({ -rect.width, 0 });
					else
						move({rect.width, 0});
				}
				else
				{
					//touching the floor, we need to move up: we're grounded
					if (colliderRect.top < rect.top)
					{
						move({ 0, -rect.height });
						grounded = true;
					}
					//touching the ceiling, we need to move downwards
					else
					{
						move({0, rect.height});
					}
				}
			}
		}
	}

	for (const auto rb : rigidBodies)
	{
		if (rb == this) continue;

		const bool wasColliding = std::find(intersectingBodies.begin(), intersectingBodies.end(), rb) != intersectingBodies.end();

		if(auto intersect = colliderRect.findIntersection(rb->colliderRect))
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

	//calculate actual velocity
	sf::Vector2f actualVelocity = (getPosition() - lastPosition) / dt;

	if(VectorUtilities::vectorLength(actualVelocity) < VectorUtilities::vectorLength(velocity))
	{
		  velocity = actualVelocity;
	}
}

void RigidBody::updateColliderRect()
{
	const sf::Vector2f position = getPosition() + colliderOffset;
	colliderRect = { {position.x - colliderSize.x / 2, position.y - colliderSize.y}, { colliderSize.x, colliderSize.y } };
}