#pragma once
#include <string>
#include <SFML/Graphics/RenderWindow.hpp>

#include "Collider.h"
#include "GameObject.h"
#include "Player.h"
#include "RigidBody.h"
#include "LDtkLoader/Project.hpp"
#include "TileMap.h"

class Scene
{
public:
	Scene(ldtk::Project* project, const std::string& levelName);
	~Scene();

	bool drawColliders = false;
	std::string name;

	std::vector<GameObject*> entities;
	std::vector<RigidBody*> rigidBodies;
	std::vector<TileMap*> tilemaps;
	std::vector<Collider*> colliders;

	Player* player;

	sf::FloatRect sceneRect;
	bool destroyed = false;

	void update();
	void draw(sf::RenderWindow* window);

	GameObject* createEntity(std::string name, sf::Vector2<float> position);
	template <class T>
	T* createEntity(std::string name, sf::Vector2<float> position);

	void addRigidBody(RigidBody* rb);
	void addEntity(GameObject* entity);
	void removeRigidBody(RigidBody* rb);
	void removeEntity(GameObject* entity);
	void destroy();
};
