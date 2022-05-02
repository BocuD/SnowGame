#include "RigidBody.h"

#include <iostream>
#include <vector>
#include <SFML/Graphics/Rect.hpp>

void RigidBody::handleCollisions(std::vector<sf::FloatRect> colliders, std::vector<RigidBody*> rigidBodies)
{
	// do collision checks
	updateColliderRect();
	auto rbCollider = colliderRect;

	for (auto& rect : colliders) {
		sf::FloatRect intersect;
		if (rbCollider.intersects(rect, intersect)) {
			if (intersect.width < intersect.height)
			{
				if (rbCollider.left < intersect.left)
					move(-intersect.width, 0);
				else
					move(intersect.width, 0);
			}
			else
			{
				//touching the floor, we need to move up: we're grounded
				if (rbCollider.top < intersect.top)
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

	for (const auto rb : rigidBodies)
	{
		if (rb == this) continue;

		sf::FloatRect intersect;
		bool colliding = std::find(intersectingBodies.begin(), intersectingBodies.end(), rb) != intersectingBodies.end();

		if(rbCollider.intersects(rb->colliderRect, intersect))
		{
			if (!colliding) 
			{
				intersectingBodies.push_back(rb);
			}
		}
		else
		{
			if (colliding) 
			{
				intersectingBodies.erase(std::remove(intersectingBodies.begin(), intersectingBodies.end(), rb), intersectingBodies.end());
			}
		}
	}
}

void RigidBody::updateColliderRect()
{
	const sf::Vector2f position = getPosition();
	colliderRect = { position.x - colliderSize.x / 2, position.y - colliderSize.y, colliderSize.x, colliderSize.y };
}
