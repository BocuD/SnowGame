#pragma once
#include <string>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Texture.hpp>

#include "Collider.h"
#include "LDtkLoader/Project.hpp"
#include "GameObject.h"
#include "TileMap.h"

class Player;
class RigidBody;

class Scene
{
public:
	Scene(ldtk::Project* project, const std::string& levelName);

	bool drawColliders = false;

	std::vector<std::unique_ptr<GameObject>> entities;
	std::vector<RigidBody*> rigidBodies;
	std::string name;
	std::vector<TileMap> tilemaps;

	std::vector<Collider> colliders;

	Player* player;

	void update();
	void draw(sf::RenderWindow* window);

	GameObject* createEntity(std::string name, sf::Vector2<float> position, sf::Texture* texture);
};