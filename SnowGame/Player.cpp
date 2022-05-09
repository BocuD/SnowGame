#include "Player.h"

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Window/Keyboard.hpp>

#include "Coin.h"
#include "Game.h"
#include "Hud.h"
#include "LoadTrigger.h"
#include "Scene.h"
#include "Snowball.h"
#include "TextureManager.h"
#include "VectorUtilities.h"

void Player::init()
{
	textureSize = { 512, 512 };
	setOrigin({ 255, 390 });
	setScale({0.15f, 0.15f});
	addTexture(TextureManager::getTexture("Assets/Sprites/BlueWizard/idle.png"), 20);
	addTexture(TextureManager::getTexture("Assets/Sprites/BlueWizard/walk.png"), 20);
	addTexture(TextureManager::getTexture("Assets/Sprites/BlueWizard/jump.png"), 8);
	addTexture(TextureManager::getTexture("Assets/Sprites/BlueWizard/dash.png"), 16);
	setTextureId(0);

	colliderSize = { 20, 35 };
	animate = true;

	health = maxHealth;

	ignorePhysics = 2;

	invincibilityFrames = 50;
}

void Player::update(float dt)
{
	if (grounded) jumpCount = 3;

	if (Game::hasFocus) {

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up))
		{
			if (onLadder > 0)
			{
				velocity.y = -100;
			}
			//only jump if we're not on a ladder
			else if (!upWasPressed && jumpCount > 0)
			{
				velocity.y = -300;
				upWasPressed = true;
				grounded = false;
				jumpCount--;

				delayFrames = 4;
				setTextureId(2);
				disableStateMachine = 8 * 4;
			}
		}
		else upWasPressed = false;

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space))
		{
			spaceWasPressed = true;
		}
		else
		{
			if (spaceWasPressed)
			{
				auto snowball = new Snowball();
				snowball->name = "Snowball";
				snowball->setPosition(getPosition());

				snowball->velocity.y = -100;

				if (velocity.x > 0)
				{
					snowball->velocity.x = velocity.x + 100;
					snowball->move({ 7, -20 });
				}
				else
				{
					snowball->velocity.x = velocity.x - 100;
					snowball->move({-7, -20});
				}

				scene->addEntity(snowball);
				scene->addRigidBody(snowball);

				spaceWasPressed = false;
			}
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::RShift))
		{
			if (!shiftWasPressed && grounded)
			{
				shiftWasPressed = true;
				velocity.x *= 4;
				disableStateMachine = 1 * 16;
				delayFrames = 1;
				setTextureId(3);
			}
		}
		else shiftWasPressed = false;
	}

	RigidBody::update(dt);
}

void Player::fixedUpdate()
{
	moving = false;

	if (Game::hasFocus) 
	{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
		{
			velocity.x -= 40;
			moving = true;
			setScale({ -0.15f, 0.15f });
		}
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
	{
		velocity.x += 40;
		moving = true;
		setScale({0.15f, 0.15f});
	}

	if (invincibilityFrames > 0)
	{
		invincibilityFrames--;

		if (invincibilityFrames > 45)
		{
			setColor({ 255, 0, 0, (sf::Uint8)((invincibilityFrames - 45) * 50) });
		}
		else
		{
			setColor({ 255, 255, 255, (sf::Uint8)((50 - invincibilityFrames) * 5) });
		}

		if (invincibilityFrames == 0)
		{
			setColor(sf::Color::White);
		}
	}

	if (disableStateMachine == 0)
	{
		if (moving && grounded)
		{
			if (textureId != 1)
			{
				delayFrames = 2;
				setTextureId(1);
			}
		}
		else
		{
			if (textureId != 0)
			{
				delayFrames = 4;
				setTextureId(0);
			}
		}
	}
	else disableStateMachine--;

	velocity.y *= 0.97f;
	velocity.x *= 0.8f;

	velocity.y += 15;

	RigidBody::fixedUpdate();
}

void Player::addHealth(int amount)
{
	health += amount;
	if(health > maxHealth)
	{
		health = maxHealth;
	}
}

void Player::removeHealth(int amount)
{
	health -= amount;
	if(health < 0)
	{
		health = 0;
		std::cout << "Player died" << std::endl;
	}
}

void Player::onCollisionEnter(RigidBody* other)
{
	if(other->name == "Mob")
	{
		if (invincibilityFrames > 0) return;

		const sf::Vector2f dir = VectorUtilities::normalizeVector(other->getPosition() - getPosition());
		velocity += -dir * 500.0f;
		invincibilityFrames = 50;
		setColor(sf::Color::Red);
		removeHealth(3);
	}
	else if(other->name == "Coin")
	{
		((Coin*)other)->pickup();
		Hud::incCoinCount();
	}
	else if (other->name == "Gem")
	{
		((Gem*)other)->pickup();
		Hud::setGemCollected(((Gem*)other)->index);
	}
}

void Player::onTriggerEnter(Collider* other)
{
	if (other->tag == "Ladder")
	{
		onLadder++;
	}
	else if(other->tag == "LoadTrigger")
	{
		if (invincibilityFrames > 0) return;

		auto trigger = (LoadTrigger*)other;
		Scene* currentScene = scene;
		int healthTemp = health;

		Game::loadScene(trigger->levelName, [trigger, currentScene, healthTemp](Scene* newScene)
		{
			Game::setActiveScene(newScene);
			newScene->player->setPosition(trigger->spawnPos);
			((Player*)newScene->player)->health = healthTemp;
			currentScene->destroy();
		});
	}
}

void Player::onTriggerExit(Collider* other)
{
	if(other->tag == "Ladder")
	{
		onLadder--;
	}
}