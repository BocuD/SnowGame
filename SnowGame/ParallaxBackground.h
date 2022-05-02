#pragma once
#include <string>
#include <vector>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>

class ParallaxLayer
{
public:
	ParallaxLayer(std::string texturePath, float depth);

	sf::Sprite sprite;
	float speed;
};

class ParallaxBackground : public sf::Drawable
{
public:
	std::vector<ParallaxLayer> layers;

private:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};