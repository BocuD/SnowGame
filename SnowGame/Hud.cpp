#include "Hud.h"

#include <iostream>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Texture.hpp>

#include "Fonts.h"
#include "TextureManager.h"

sf::Texture heartTexture[2];
sf::Sprite coin;
sf::Text coinText;
sf::Text coinTextBG;

void Hud::init()
{
	heartTexture[0].loadFromFile("Assets/Sprites/Hearts/animated/border/heart_animated_1.png");
	heartTexture[1].loadFromFile("Assets/Sprites/Hearts/animated/border/heart_animated_2.png");

	coin.setTexture(*TextureManager::getTexture("Assets/Coin_Gems/MonedaD.png"));
	coin.setTextureRect({ 0, 0, 16, 16 });
	coin.setPosition(15, 65);
	coin.setScale(2, 2);

	coinText.setFont(Fonts::mainFont);
	coinText.setString("x 0");
	coinText.setPosition(52, 76);
	coinText.setScale(0.6f, 0.6f);

	coinTextBG.setFont(Fonts::mainFont);
	coinTextBG.setString("x 0");
	coinTextBG.setPosition(54, 78);
	coinTextBG.setScale(0.6f, 0.6f);

	coinTextBG.setFillColor(sf::Color::Black);

	for (int i = 0; i < 5; i++)
	{
		hearts[i].setTexture(heartTexture[0]);
		hearts[i].setScale(3, 3);
		hearts[i].setPosition(5 + i * 60, 5);
		hearts[i].setTextureRect({ 0, 0, 17, 17 });
		hearts[i].setColor(sf::Color::Transparent);
	}
}

int slowdown = 0;
int visualHealth = 0;

void Hud::update()
{
	if (visualHealth != health)
	{
		slowdown++;
		if (slowdown != 4) return;
		slowdown = 0;

		int heart, step;

		if (visualHealth > health)
		{
			visualHealth--;

			step = (visualHealth + 1) % 5;
			heart = (visualHealth) / 5;

			updateHeartSprite(heart, step, true);
		}
		else if (visualHealth < health)
		{
			visualHealth++;

			step = (visualHealth + 1) % 5;
			heart = (visualHealth - 1) / 5;

			updateHeartSprite(heart, step, false);
		}
	}
	else slowdown = 0;
}

int coins = 0;
std::string coinTextString;

void Hud::incCoinCount()
{
	coins++;
	coinTextString = "x " + std::to_string(coins);
	coinText.setString(coinTextString);
	coinTextBG.setString(coinTextString);
}

const int heartLeft[] = { 17, 0, 68, 51, 34 };
void Hud::updateHeartSprite(int heart, int step, bool moveDown)
{
	if (step == 2) hearts[heart].setColor(sf::Color::White);
	if (step == 1 && moveDown) hearts[heart].setColor(sf::Color::Transparent);

	hearts[heart].setTextureRect({ heartLeft[step], 0, 17, 17 });
}

void Hud::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	for (int i = 0; i < 5; i++)
	{
		target.draw(hearts[i]);
	}

	target.draw(coin);
	target.draw(coinTextBG);
	target.draw(coinText);
}
