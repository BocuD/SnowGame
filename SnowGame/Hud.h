#pragma once
#include <SFML/Graphics/Sprite.hpp>

class Hud : public sf::Drawable
{
public:
	sf::Sprite* hearts[5];
	void init();
	void updateHeartSprite(int heart, int step, bool moveDown);
	void update();
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	int health = 0;
};
