#pragma once
#include <string>
#include <vector>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>

class ParallaxBackground : public sf::Drawable
{
public:
	std::vector<sf::Sprite> sprites;
	std::vector<sf::Texture> textures;

	void addLayer(std::string texturePath);
	void update(sf::Vector2f camPos);

	sf::Vector2f offset;

private:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};