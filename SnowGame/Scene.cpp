#include "Scene.h"

#include <utility>
#include <SFML/Graphics/RectangleShape.hpp>

#include "Mob.h"
#include "ParallaxBackground.h"
#include "Player.h"

namespace sf
{
	class RectangleShape;
}

sf::RectangleShape getColliderShape(const sf::FloatRect& rect, sf::Color c);

Scene::Scene(ldtk::Project* project, const std::string& levelName)
{
	auto* level = &project->getWorld().getLevel(levelName);
	const auto& layers = level->allLayers();

	for (const auto& layer : layers)
	{
		if (layer.getName() == "Collisions")
		{
			for (ldtk::Entity& col : layer.getEntitiesByName("Wall"))
			{
				Collider c;
				c.bounds = { (float)col.getPosition().x - (float)col.getSize().x / 2, (float)col.getPosition().y - (float)col.getSize().y ,
					(float)col.getSize().x, (float)col.getSize().y };
				c.tag = "Wall";
				c.isTrigger = false;
				colliders.push_back(c);
			}

			for (ldtk::Entity& col : layer.getEntitiesByName("Ladder"))
			{
				Collider c;
				c.bounds = { (float)col.getPosition().x - (float)col.getSize().x / 2, (float)col.getPosition().y - (float)col.getSize().y ,
					(float)col.getSize().x, (float)col.getSize().y };
				c.tag = "Ladder";
				c.isTrigger = true;
				colliders.push_back(c);
			}
		}
		else
		{
			for (const auto& entity : layer.allEntities())
			{
				std::cout << "Creating entity " << entity.getName() << std::endl;
				auto position = entity.getPosition();

				if (entity.getName() == "Player")
				{
					auto p = std::make_unique<Player>();
					p->init();
					p->setPosition((float)position.x, (float)position.y);
					p->name = "Player";

					entities.push_back(std::move(p));
					player = (Player*)entities.back().get();

					rigidBodies.push_back((RigidBody*)player);
				}
				else if (entity.getName() == "Mob")
				{
					auto m = std::make_unique<Mob>();
					m->init();
					m->setPosition((float)position.x, (float)position.y);
					m->name = entity.getName();

					entities.push_back(std::move(m));
					rigidBodies.push_back((RigidBody*)entities.back().get());
				}
			}
		}

		if (!layer.isVisible()) continue;

		const ldtk::Tileset* tileSet = &layer.getTileset();
		//layer without tiles, probably entity layer
		if (tileSet == nullptr)
		{
			continue;
		}

		unsigned levelWidth = level->size.x / layer.getCellSize();
		unsigned levelHeight = level->size.y / layer.getCellSize();

		const size_t tileCount = levelWidth * levelHeight;
		std::vector<ldtk::Tile> tiles;

		for (unsigned y = 0; y < levelHeight; y++)
		{
			for (unsigned x = 0; x < levelWidth; x++)
			{
				tiles.push_back(layer.getTile(x, y));
			}
		}

		TileMap map;
		bool success = map.load("Assets/" + tileSet->path, sf::Vector2u(tileSet->tile_size, tileSet->tile_size), tiles, levelWidth, levelHeight);
		if (success)
			std::cout << "Successfully loaded layer " << layer.getName() << std::endl;

		map.setScale(1, 1);

		tilemaps.push_back(map);
	}
	std::cout << "Loaded level " << level->name << std::endl;
	name = level->name;
}

void Scene::update()
{
	for (const auto& entity : entities)
	{
		entity->update();
	}

	//run collision detection
	for (auto rb : rigidBodies)
		rb->handleCollisions(colliders, rigidBodies);
}

void Scene::draw(sf::RenderWindow* window)
{
	//draw tiles
	for (TileMap map : tilemaps)
		window->draw(map);

	//draw entities
	for (const auto& entity : entities)
		window->draw(*entity);


	if (drawColliders) 
	{
		//draw rigidbody boxes
		for (auto rb : rigidBodies)
		{
			rb->updateColliderRect();
			window->draw(getColliderShape(rb->colliderRect, sf::Color::Red));
		}

		//draw collider boxes
		for (auto& col : colliders)
			window->draw(getColliderShape(col.bounds, sf::Color::Green));
	}
}

sf::RectangleShape getColliderShape(const sf::FloatRect& rect, sf::Color c)
{
	sf::RectangleShape r{ {rect.width, rect.height} };
	r.setPosition(rect.left, rect.top);
	r.setFillColor({c.r, c.b, c.g, 100});
	return r;
}

GameObject* Scene::createEntity(std::string name, sf::Vector2<float> position, sf::Texture* texture = nullptr)
{
	std::unique_ptr<GameObject> entity = std::make_unique<GameObject>();

	entity->name = name;
	entity->setPosition(position);
	entity->init();

	if(texture != nullptr)
	{
		entity->setTexture(*texture);
	}

	entities.push_back(std::move(entity));

	return entities.back().get();
}

void Scene::removeEntity(GameObject* gameObject)
{
	entities.erase(std::remove(entities.begin(), entities.end(), &gameObject), entities.end());
}
