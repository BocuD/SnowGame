#include "ParallaxBackground.h"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Texture.hpp>

void ParallaxBackground::addLayer(std::string texturePath)
{
	textures.emplace_back();
	sf::Texture* texture = &textures.back();
	texture->loadFromFile(texturePath);
	texture->setRepeated(true);

	sprites.emplace_back();
	sf::Sprite* sprite = &sprites.back();
	sprite->setTexture(*texture);
	sprite->setScale(5, 5);
	sf::IntRect rect = sprite->getTextureRect();
	sprite->setTextureRect({ 0, 0, 2048, rect.height });
}

void ParallaxBackground::update(const sf::Vector2f camPos)
{
	for(size_t i = 0; i < sprites.size(); i++)
	{
		sprites[i].setPosition(camPos * (float)i + offset);
	}
}

void ParallaxBackground::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	for (sf::Sprite layer: sprites)
	{
		target.draw(layer);
	}
}
