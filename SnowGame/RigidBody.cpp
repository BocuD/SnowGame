#include "RigidBody.h"

#include <vector>
#include <SFML/Graphics/Rect.hpp>

void RigidBody::handleCollisions(std::vector<sf::FloatRect> colliders)
{
	// do collision checks
	auto rbCollider = getColliderRect();

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
}

sf::FloatRect RigidBody::getColliderRect()
{
	return {0, 0, 0, 0};
}
