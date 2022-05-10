#include "Scene.h"

#include <SFML/Graphics/RectangleShape.hpp>

#include "DemoCamera.h"
#include "Coin.h"
#include "Game.h"
#include "LoadTrigger.h"
#include "Mob.h"
#include "ParallaxBackground.h"
#include "Player.h"

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
				Collider* c = new Collider;
				c->bounds = { {(float)col.getPosition().x - (float)col.getSize().x / 2, (float)col.getPosition().y - (float)col.getSize().y },
				{ (float)col.getSize().x, (float)col.getSize().y }};
				c->tag = "Wall";
				c->isTrigger = false;
				colliders.push_back(c);
			}

			for (ldtk::Entity& col : layer.getEntitiesByName("Ladder"))
			{
				Collider* c = new Collider;
				c->bounds = { {(float)col.getPosition().x - (float)col.getSize().x / 2, (float)col.getPosition().y - (float)col.getSize().y },
				{ (float)col.getSize().x, (float)col.getSize().y } };

				c->tag = "Ladder";
				c->isTrigger = true;
				colliders.push_back(c);
			}

			for (ldtk::Entity& col : layer.getEntitiesByName("LoadTrigger"))
			{
				LoadTrigger* trigger = new LoadTrigger;
				trigger->bounds = { {(float)col.getPosition().x - (float)col.getSize().x / 2, (float)col.getPosition().y - (float)col.getSize().y },
				{ (float)col.getSize().x, (float)col.getSize().y } };

				trigger->tag = "LoadTrigger";
				trigger->levelName = col.getField<std::string>("levelName").value();
				trigger->spawnPos = { col.getField<float>("xPos").value(), col.getField<float>("yPos").value() };
				trigger->isTrigger = true;
				colliders.push_back(trigger);
			}
		}
		else
		{
			for (auto& entity : layer.allEntities())
			{
				std::cout << "Creating entity " << entity.getName() << std::endl;
				sf::Vector2f position = { (float)entity.getPosition().x, (float)entity.getPosition().y };

				if (entity.getName() == "Camera")
				{
					player = createEntity<DemoCamera>(entity);
					addRigidBody((RigidBody*)player);
				}
				else if (entity.getName() == "Player")
				{
					player = createEntity<Player>(entity);
					addRigidBody((RigidBody*)player);
				}
				else if (entity.getName() == "Mob")
				{
					auto m = createEntity<Mob>(entity);
					addRigidBody(m);
				}
				else if (entity.getName() == "Coin")
				{
					auto c = createEntity<Coin>(entity);
					addRigidBody(c);
				}
				else if (entity.getName() == "Gem")
				{
					auto c = createEntity<Gem>(entity);
					addRigidBody(c);
				}
				else if (entity.getName() == "Music")
				{
					ldtk::FilePath path = entity.getField<ldtk::FilePath>("MusicPath").value();
					musicName = "Assets/" + path.directory() + path.filename();

					music = new sf::Music();
					if(music->openFromFile(musicName))
					{
						music->play();
						music->setLoop(true);
					}
					else
					{
						std::cout << "Failed to load music clip " << musicName << std::endl;
					}
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

		TileMap* map = new TileMap;
		bool success = map->load(TextureManager::getTexture("Assets/" + tileSet->path), sf::Vector2u(tileSet->tile_size, tileSet->tile_size), tiles, levelWidth, levelHeight);
		if (success)
			std::cout << "Successfully loaded layer " << layer.getName() << std::endl;

		tilemaps.push_back(map);
	}
	std::cout << "Loaded level " << level->name << std::endl;
	name = level->name;
	sceneRect = { {0, 0}, {(float)level->size.x, (float)level->size.y} };
}

Scene::~Scene()
{
	for (auto& tilemap : tilemaps)
	{
		delete tilemap;
	}

	for (auto& obj : entities)
	{
		delete obj;
	}

	for (auto& col : colliders)
	{
		delete col;
	}
}

void Scene::update(float dt)
{
	//run physics at full framerate for smoother animations
	for (auto& rb : rigidBodies)
	{
		rb->physicsTick(colliders, rigidBodies, dt);
	}

	//update entities
	size_t entityCount = entities.size();
	for (int i = 0; i < entityCount; i++)
	{
		entities[i]->update(dt);
	}

	//handle object deletion
	for (auto& rb : rigidBodies)
	{
		if (rb->destroyed)
		{
			removeRigidBody(rb);
		}
	}

	for (auto& entity : entities)
	{
		if (entity->destroyed)
		{
			entity->onDestroy();
			delete entity;
			removeEntity(entity);
		}
	}
}

void Scene::fixedUpdate()
{
	for (auto& entity : entities)
	{
		entity->fixedUpdate();
	}
}

void Scene::draw(sf::RenderWindow* window)
{
	//draw tiles
	for (auto map : tilemaps)
		window->draw(*map);

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
		{
			window->draw(getColliderShape(col->bounds, sf::Color::Green));
		}
	}
}

sf::RectangleShape getColliderShape(const sf::FloatRect& rect, sf::Color c)
{
	sf::RectangleShape r{ {rect.width, rect.height} };
	r.setPosition({ rect.left, rect.top });
	r.setFillColor({c.r, c.b, c.g, 100});
	return r;
}

template <class T>

T* Scene::createEntity(const ldtk::Entity& entity)
{
	auto* newEntity = new T;

	newEntity->name = entity.getName();

	sf::Vector2f pos = { (float)entity.getPosition().x, (float)entity.getPosition().y };
	newEntity->setPosition(pos);
	newEntity->loadEntityData(entity);

	addEntity(newEntity);

	return (T*)entities.back();
}

void Scene::addRigidBody(RigidBody* rb)
{
	rigidBodies.push_back(rb);
}

void Scene::addEntity(GameObject* entity)
{
	std::cout << "adding entity to scene: " << entity << std::endl;
	entity->scene = this;
	entities.push_back(entity);
	entity->init();
}

void Scene::removeRigidBody(RigidBody* rb)
{
	rigidBodies.erase(std::remove(rigidBodies.begin(), rigidBodies.end(), rb), rigidBodies.end());
}

void Scene::removeEntity(GameObject* entity)
{
	std::cout << "removing entity from scene: " << entity << std::endl;
	entities.erase(std::remove(entities.begin(), entities.end(), entity), entities.end());
}

void Scene::destroy()
{
	destroyed = true;
}
