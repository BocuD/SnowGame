#include "Menu.h"

#include <SFML/Graphics/RenderWindow.hpp>

#include "Game.h"

void alignCenter(ShadedText* text, sf::Vector2f position);

Menu::Menu()
{
	titleText = new ShadedText("Snowball Jump", Fonts::mainFont, sf::Color::White, sf::Color::Black, 60);
	startGameText = new ShadedText("Start game", Fonts::mainFont, sf::Color::White, sf::Color::Black, 30);
	optionsText = new ShadedText("Options", Fonts::mainFont, sf::Color::White, sf::Color::Black, 30);
	creditsText = new ShadedText("Credits", Fonts::mainFont, sf::Color::White, sf::Color::Black, 30);

	cursorText = new ShadedText(">", Fonts::mainFont, sf::Color::White, sf::Color::Black, 30);

	alignCenter(titleText, {0, -100});
	alignCenter(startGameText, { 0, 50 });
	alignCenter(optionsText, { 0, 100 });
	alignCenter(creditsText, { 0, 150 });

	cursorText->setPosition({ -130, 50 });

	menuState = MenuState::title;
}

void Menu::handleInput(const sf::Event::KeyEvent& key)
{
	switch(key.code)
	{
	case sf::Keyboard::Key::Up:
		currentOption--;
		if (currentOption < 0) currentOption = 3;
		break;
	case sf::Keyboard::Key::Down:
		currentOption++;
		if (currentOption > 2) currentOption = 0;
		break;
	case sf::Keyboard::Key::Enter:
		Game::startGame();
		break;
	}

	cursorText->setPosition({ -130, (float)50 + currentOption * 50 });
}

void Menu::selectOption()
{
	switch(menuState)
	{
	case MenuState::title:
		switch(currentOption)
		{
		case 0:
			Game::startGame();
			break;
		case 1:
			currentOption = 0;
			menuState = MenuState::options;
			break;
		case 2:
			currentOption = 0;
			menuState = MenuState::credits;
			break;
		}
		break;
	case MenuState::options: break;
	case MenuState::loadSave: break;
	case MenuState::credits: break;
	}
}

void Menu::draw(sf::RenderWindow* window)
{
	switch (menuState) {
	case MenuState::title:
		window->draw(*titleText);
		window->draw(*startGameText);
		window->draw(*optionsText);
		window->draw(*creditsText);
		break;
	case MenuState::options:
		break;
	case MenuState::credits:
		break;
	}

	window->draw(*cursorText);
}

void alignCenter(ShadedText* text, sf::Vector2f position)
{
	sf::FloatRect bounds = text->getLocalBounds();
	text->setPosition({ -bounds.width / 2 + position.x, position.y });
}