#pragma once

#include <SFML/Window/Event.hpp>

#include "Fonts.h"
#include "Scene.h"

class Menu : public sf::Drawable
{
public:
	Menu();
	void setTitle(std::string title, int textSize);
	void addOption(std::string title, std::function<void()> onSelection);
	void handleInput(const sf::Event::KeyEvent& key);
	void selectOption(int option);

	int currentOption = 0;
	int optionCount = 0;
	float menuWidth = 0;

	static void alignCenter(ShadedText* text, sf::Vector2f position);

private:
	std::vector<ShadedText> text;
	std::vector<std::function<void()>> onSelect;

	ShadedText* cursorText;
	ShadedText* titleText;

	void draw(sf::RenderTarget& target, const sf::RenderStates& states) const override;
};
