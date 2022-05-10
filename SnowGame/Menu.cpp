#include "Menu.h"
#include <SFML/Graphics/RenderWindow.hpp>
#include "Game.h"

Menu::Menu()
{
	titleText = new ShadedText("", Fonts::mainFont, sf::Color::White, sf::Color::Black, 60);
	
	cursorText = new ShadedText(">", Fonts::mainFont, sf::Color::White, sf::Color::Black, 30);
	cursorText->setPosition({ -130, 50 });
}

void Menu::setTitle(std::string title, int textSize)
{
	if (titleText != nullptr)
		delete titleText;

	titleText = new ShadedText(title, Fonts::mainFont, sf::Color::White, sf::Color::Black, textSize);
	alignCenter(titleText, { 0, -70 });
}

void Menu::addOption(std::string title, std::function<void()> onSelection)
{
	text.emplace_back(title, Fonts::mainFont, sf::Color::White, sf::Color::Black, 30);
	onSelect.push_back(onSelection);
	optionCount++;
	alignCenter(&text.back(), { 0, (float) 50 * optionCount });

	if(text.back().getLocalBounds().width > menuWidth)
	{
		menuWidth = text.back().getLocalBounds().width;
		cursorText->setPosition({ -menuWidth / 2 - 30, (float)50 + currentOption * 50 });
	}
}

void Menu::handleInput(const sf::Event::KeyEvent& key)
{
	switch(key.code)
	{
	case sf::Keyboard::Key::Up:
		currentOption--;
		if (currentOption < 0) currentOption = optionCount - 1;
		break;
	case sf::Keyboard::Key::Down:
		currentOption++;
		if (currentOption > optionCount - 1) currentOption = 0;
		break;
	case sf::Keyboard::Key::Enter:
		selectOption(currentOption);
		break;
	}

	cursorText->setPosition({ -menuWidth / 2 - 30, (float)50 + currentOption * 50 });
}

void Menu::selectOption(int option)
{
	onSelect[option]();
}

void Menu::alignCenter(ShadedText* text, sf::Vector2f position)
{
	sf::FloatRect bounds = text->getLocalBounds();
	text->setPosition({ -bounds.width / 2 + position.x, position.y });
}

void Menu::draw(sf::RenderTarget& target, const sf::RenderStates& states) const
{
	for (auto& element : text)
	{
		target.draw(element);
	}

	if (titleText != nullptr) target.draw(*titleText);

	if (optionCount > 0)
		target.draw(*cursorText);
}