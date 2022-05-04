#include "GameObject.h"

#include "Scene.h"

void GameObject::destroy()
{
	destroyed = true;
	std::cout << "scheduling entity removal " << this << std::endl;

	//scene->removeEntity(this);
	//delete this;
}
