#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Window/Event.hpp>

#include "Fonts.h"

class Menu
{
public:
	Menu();
	void handleInput(const sf::Event::KeyEvent& key);
	void selectOption();
	void draw(sf::RenderWindow* window);

	int currentOption = 0;

	enum class MenuState
	{
		title,
		options,
		loadSave,
		credits
	};

	MenuState menuState;

private:
	ShadedText* titleText;
	ShadedText* startGameText;
	ShadedText* optionsText;
	ShadedText* creditsText;

	ShadedText* cursorText;
};
