#include "Player.h"

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Window/Keyboard.hpp>
#include "VectorUtilities.h"

void Player::init()
{
	textureSize = { 512, 512 }; 
	setOrigin(255, 400);
	setScale(0.15f, 0.15f);
	addTexture("Assets/Sprites/BlueWizard/idle.png", 20);
	addTexture("Assets/Sprites/BlueWizard/walk.png", 20);
	addTexture("Assets/Sprites/BlueWizard/jump.png", 8);
	addTexture("Assets/Sprites/BlueWizard/dash.png", 16);
	setTextureId(0);

	colliderSize = { 20, 40 };
	animate = true;

	health = maxHealth;
}

bool upWasPressed = false;
bool shiftWasPressed = false;
sf::Vector2f velocity;
int disableStateMachine = 0;
int jumpCount = 3;
int invincibilityFrames = 0;
int frameMod;

void Player::runStateMachine(bool moving)
{
	if (invincibilityFrames > 0)
	{
		invincibilityFrames--;
		
		if (invincibilityFrames > 45)
		{
			setColor({ 255, 0, 0, (sf::Uint8)((invincibilityFrames - 45) * 50)});
		}
		else
		{
			setColor({255, 255, 255, (sf::Uint8)((50 - invincibilityFrames) * 5)});
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
				delayFrames = 3;
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
}

void Player::update()
{
	bool moving = false;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
	{
		velocity.x -= 30;
		moving = true;
		setScale(-0.15f, 0.15f);
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
	{
		velocity.x += 30;
		moving = true;
		setScale(0.15f, 0.15f);
	}

	runStateMachine(moving);

	if (grounded) jumpCount = 3;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up)) {
		if (!upWasPressed && jumpCount > 0) {
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
		
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down))
		move(0, 1);

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
	const float dt = 1 / 60.0f;

	velocity.y *= 0.93f;
	velocity.x *= 0.8f;

	velocity.y += 700 * dt;

	move(velocity * dt);

	RigidBody::update();
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
	if (invincibilityFrames > 0) return;

	if(other->name == "Mob")
	{
		const sf::Vector2f dir = VectorUtilities::normalizeVector(other->getPosition() - getPosition());
		velocity += -dir * 500.0f;
		invincibilityFrames = 50;
		setColor(sf::Color::Red);
		removeHealth(3);
	}
}
