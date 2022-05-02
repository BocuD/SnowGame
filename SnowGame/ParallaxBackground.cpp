#include "ParallaxBackground.h"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Texture.hpp>

ParallaxLayer::ParallaxLayer(std::string texturePath, float depth)
{
	/*sf::Texture texture;
	texture.loadFromFile(texturePath);

	sprite.setTexture(texture);
	sprite.setOrigin(texture.getSize().x/2, texture.getSize().y/2);
	speed = 0;*/

	sf::Texture t;
	t.loadFromFile("Assets/Backgrounds/Glacial-mountains/Layers/sky.png");
	sprite.setTexture(t);
	sprite.setScale(5, 5);
}

void ParallaxBackground::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	for (auto layer : layers)
	{
		target.draw(layer.sprite);
	}
}