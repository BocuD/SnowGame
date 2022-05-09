#include "Hud.h"

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Texture.hpp>

#include "Fonts.h"
#include "TextureManager.h"

sf::Texture heartTexture[2];

sf::Sprite coin;
sf::Text coinText;
sf::Text coinTextBG;

sf::Sprite gem[3];

void Hud::init()
{
	heartTexture[0].loadFromFile("Assets/Sprites/Hearts/animated/border/heart_animated_1.png");
	heartTexture[1].loadFromFile("Assets/Sprites/Hearts/animated/border/heart_animated_2.png");

	coin.setTexture(*TextureManager::getTexture("Assets/Coin_Gems/MonedaD.png"));
	coin.setTextureRect({ { 0, 0 }, {16, 16} });
	coin.setPosition({ 11, 50 });
	coin.setScale({ 2, 2 });

	coinText.setFont(Fonts::mainFont);
	coinText.setString("x 0");
	coinText.setPosition({ 48, 61 });
	coinText.setScale({0.6f, 0.6f});

	coinTextBG.setFont(Fonts::mainFont);
	coinTextBG.setString("x 0");
	coinTextBG.setPosition({ 50, 63 });
	coinTextBG.setScale({ 0.6f, 0.6f });

	coinTextBG.setFillColor(sf::Color::Black);

	for (int i = 0; i < 5; i++)
	{
		hearts[i].setTexture(heartTexture[0]);
		hearts[i].setScale({ 2, 2 });
		hearts[i].setPosition({ 10 + (float)i * 40, 10 });
		hearts[i].setTextureRect({ { 0, 0 }, {17, 17} });
		hearts[i].setColor(sf::Color::Transparent);
	}

	sf::Texture* gemTex = TextureManager::getTexture("Assets/Coin_Gems/spr_coin_strip4.png");

	for (int i = 0; i < 3; i++)
	{
		gem[i].setTexture(*gemTex);
		gem[i].setScale({ 2, 2 });
		gem[i].setColor({ 255, 255, 255, 100 });
		gem[i].setTextureRect({{ 0, 0 }, { 16, 16}});
		gem[i].setPosition({ 90 + (float)i * 27, 49 });
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

void Hud::setGemCollected(int i)
{
	gem[i].setColor(sf::Color::White);
}

void Hud::draw(sf::RenderTarget& target, const sf::RenderStates& states) const
{
	for (int i = 0; i < 5; i++)
	{
		target.draw(hearts[i]);
	}

	for (int i = 0; i < 3; i++)
	{
		target.draw(gem[i]);
	}

	target.draw(coin);
	target.draw(coinTextBG);
	target.draw(coinText);
}

const int heartLeft[] = { 17, 0, 68, 51, 34 };
void Hud::updateHeartSprite(int heart, int step, bool moveDown)
{
	if (step == 2) hearts[heart].setColor(sf::Color::White);
	if (step == 1 && moveDown) hearts[heart].setColor(sf::Color::Transparent);

	hearts[heart].setTextureRect({ {heartLeft[step], 0}, {17, 17 } });
}
