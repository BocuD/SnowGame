#include "Player.h"
#include <SFML/Window/Keyboard.hpp>

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
}

bool upWasPressed = false;
bool shiftWasPressed = false;
sf::Vector2f velocity;
int disableStateMachine = 0;
int jumpCount = 3;

void Player::update()
{
	bool moving = false;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
	{
		velocity.x -= 30;
		moving = true;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
	{
		velocity.x += 30;
		moving = true;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space))
	{
		std::cout << "Space button pressed" << std::endl;
		velocity.y -= 200;
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
		
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down))
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

	if(velocity.x > 0) setScale(0.15f, 0.15f);
	else setScale(-0.15f, 0.15f);

	move(velocity * dt);

	//if (getPosition().y >= 144) {
	//	//velocity.y = 0;
	//	setPosition(getPosition().x, 144);
	//	//isGrounded = true;
	//	jumpCount = 3;
	//}

	RigidBody::update();
}

void Player::onCollisionEnter(RigidBody* other)
{
	if(other->name == "Mob")
	{
		std::cout << "touched a mob, bad" << std::endl;
	}
}
