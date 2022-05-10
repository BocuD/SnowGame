#include "Player.h"

#include <SFML/Window/Keyboard.hpp>

#include "Coin.h"
#include "Game.h"
#include "Hud.h"
#include "LoadTrigger.h"
#include "Mob.h"
#include "Scene.h"
#include "SFXManager.h"
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

	invincibilityFrames = 35;

	jump.setBuffer(*SFXManager::getSoundBuffer("Assets/SFX/jump.ogg"));
	hit.setBuffer(*SFXManager::getSoundBuffer("Assets/SFX/hit.ogg"));
	teleport.setBuffer(*SFXManager::getSoundBuffer("Assets/SFX/teleport.ogg"));
	coin.setBuffer(*SFXManager::getSoundBuffer("Assets/SFX/coin.ogg"));
	gem.setBuffer(*SFXManager::getSoundBuffer("Assets/SFX/gem.ogg"));
	snowball.setBuffer(*SFXManager::getSoundBuffer("Assets/SFX/snowball.ogg"));
	land.setBuffer(*SFXManager::getSoundBuffer("Assets/SFX/land.ogg"));
	dash.setBuffer(*SFXManager::getSoundBuffer("Assets/SFX/dash.ogg"));
}

void Player::update(float dt)
{
	if (grounded && !wasGrounded) 
	{
		land.play();
		jumpCount = 3;
	}

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
				jump.play();
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
				snowball.play();
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
				dash.play();
			}
		}
		else shiftWasPressed = false;
	}

	RigidBody::update(dt);

	wasGrounded = grounded;
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
		Game::endGame();
	}
}

void Player::onCollisionEnter(RigidBody* other)
{
	if(other->name == "Mob")
	{
		if (invincibilityFrames > 0) return;
		if (((Mob*)other)->dieCounter > 0) return;

		hit.play();
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
		coin.play();
	}
	else if (other->name == "Gem")
	{
		((Gem*)other)->pickup();
		Hud::setGemCollected(((Gem*)other)->index);
		gem.play();
	}
}

void Player::onTriggerEnter(Collider* other)
{
	if (other->tag == "Ladder")
	{
		onLadder++;
	}
	else if (other->tag == "LoadTrigger")
	{
		if (invincibilityFrames > 0) return;

		teleport.play();
		auto trigger = (LoadTrigger*)other;
		Scene* currentScene = scene;
		int healthTemp = health;

		//first try to loop through loaded scenes, and if the target scene is loaded, just set it as active
		for (auto& newScene : Game::scenes)
		{
			if (newScene->name == trigger->levelName)
			{
				Game::setActiveScene(newScene.get());
				newScene->enabled = true;
				Player* player = (Player*)newScene->player;

				player->setPosition(trigger->spawnPos);
				player->health = healthTemp;
				player->ignorePhysics = 2;
				player->invincibilityFrames = 35;

				if (currentScene->musicName == newScene->musicName) 
				{
					if (currentScene->music != newScene->music)
					{
						delete newScene->music;
						newScene->music = currentScene->music;
					}
				}
				else
				{
					currentScene->music->stop();
					newScene->music->setPlayingOffset(sf::seconds(0));
					newScene->music->play();
				}
				currentScene->enabled = false;
				return;
			}
		}

		//if the scene isn't loaded, load it
		Game::loadScene(trigger->levelName, [trigger, currentScene, healthTemp](Scene* newScene)
		{
			Game::setActiveScene(newScene);
			Player* player = (Player*)newScene->player;

			player->setPosition(trigger->spawnPos);
			player->health = healthTemp;

			if (currentScene->musicName == newScene->musicName)
			{
				delete newScene->music;
				newScene->music = currentScene->music;
			}
			else
			{
				currentScene->music->stop();
				newScene->music->setPlayingOffset(sf::seconds(0));
				newScene->music->play();
			}
			currentScene->enabled = false;
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