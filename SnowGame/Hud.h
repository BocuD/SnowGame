#pragma once
#include <SFML/Graphics/Sprite.hpp>

class Hud : public sf::Drawable
{
public:
	sf::Sprite* hearts = new sf::Sprite[5];
	void init();
	void updateHeartSprite(int heart, int step, bool moveDown);
	void update();
	static void incCoinCount();
	static void setGemCollected(int i);

	void draw(sf::RenderTarget& target, const sf::RenderStates& states) const override;

	int health = 0;
};
