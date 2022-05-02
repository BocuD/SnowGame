#include "GameObject.h"

void GameObject::init()
{
	
}

void GameObject::update()
{
	/*for (const auto& component : components)
	{
		component->update();
	}*/
}

bool GameObject::checkOverlap(GameObject* other)
{
	sf::Vector2f thisObjectSize = { (float)getTextureRect().width, (float)getTextureRect().height };
	sf::Vector2f otherObjectSize = { (float)other->getTextureRect().width, (float)other->getTextureRect().height };

	sf::Vector2f velocity;
	sf::Rect <float> test;
	//test.intersects();
	return true;
}