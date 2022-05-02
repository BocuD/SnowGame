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

ParallaxBackground bg;

sf::RectangleShape getColliderShape(const sf::FloatRect& rect, sf::Color c);

Scene::Scene(std::string filepath, const std::string& levelName)
{
	ldtk::Project project;
	project.loadFromFile(filepath);

	std::cout << "Successfully loaded project file " << filepath << std::endl;

	const auto& world = project.getWorld();
	const auto& level = world.getLevel(levelName);
	const auto& layers = level.allLayers();

	for (size_t i = 0; i < layers.size(); i++)
	{
		const auto& layer = layers[i];

		if(layer.getName() == "Collisions")
		{
			for (ldtk::Entity& col : layer.getEntitiesByName("Wall")) {
				colliders.emplace_back((float)col.getPosition().x - (float) col.getSize().x / 2, (float)col.getPosition().y - (float)col.getSize().y ,
					(float)col.getSize().x, (float)col.getSize().y);
			}
		}
		else 
		{
			for (const auto& entity : layer.allEntities())
			{
				std::cout << "Creating entity " << entity.getName() << std::endl;
				if (entity.getName() == "Player")
				{
					auto p = std::make_unique<Player>();
					p->setPosition(sf::Vector2f(entity.getPosition().x, entity.getPosition().y));
					p->init();

					entities.push_back(std::move(p));

					player = entities.back().get();
					rigidBodies.push_back((RigidBody*)player);
				}
				else if (entity.getName() == "Mob")
				{
					auto m = std::make_unique<Mob>();

					m->init();
					m->setPosition(sf::Vector2f(entity.getPosition().x, entity.getPosition().y));
					m->name = entity.getName();

					entities.push_back(std::move(m));
					rigidBodies.push_back((RigidBody*)entities.back().get());
				}
			}
		}

		const ldtk::Tileset* tileSet = &layer.getTileset();
		//layer without tiles, probably entity layer
		if (tileSet == nullptr)
		{
			continue;
		}

		unsigned levelWidth = level.size.x / layer.getCellSize();
		unsigned levelHeight = level.size.y / layer.getCellSize();

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
	std::cout << "Loaded scene " << level.name << std::endl;
	name = level.name;

	std::cout << "Scene Entity Dump:" << std::endl;
	for (const auto &entity : entities)
	{
		std::cout << entity->name << std::endl;
	}

	/*ParallaxLayer layer0("Assets/Backgrounds/Glacial-mountains/Layers/sky.png", 0);
	ParallaxLayer layer1("Assets/Backgrounds/Glacial-mountains/Layers/clouds_bg.png", 0);
	ParallaxLayer layer2("Assets/Backgrounds/Glacial-mountains/Layers/glacial_mountains.png", 0);

	bg.layers.push_back(layer0);
	bg.layers.push_back(layer1);
	bg.layers.push_back(layer2);*/
}

void Scene::update()
{
	for (const auto& entity : entities)
	{
		entity->update();
	}

	//run collision detection
	for (auto rb : rigidBodies)
		rb->handleCollisions(colliders);
}

void Scene::draw(sf::RenderWindow* window)
{
	//window->draw(bg);

	//draw tiles
	for (TileMap map : tilemaps)
		window->draw(map);

	//draw entities
	for (const auto& entity : entities)
		window->draw(*entity);

	//draw rigidbody boxes
	for (auto rb : rigidBodies)
		window->draw(getColliderShape(rb->getColliderRect(), sf::Color::Red));

	//draw collider boxes
	for (auto& rect : colliders)
		window->draw(getColliderShape(rect, sf::Color::Green));
}

sf::RectangleShape getColliderShape(const sf::FloatRect& rect, sf::Color c)
{
	sf::RectangleShape r{ {rect.width, rect.height} };
	r.setPosition(rect.left, rect.top);
	r.setFillColor({c.r, c.b, c.g, 100});
	return r;
}

GameObject* Scene::createEntity(std::string name, EntityType type, sf::Vector2<float> position, sf::Texture* texture = nullptr)
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
