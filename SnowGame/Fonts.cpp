#include "Fonts.h"

#include <SFML/Graphics/RenderTarget.hpp>

sf::Font Fonts::mainFont = sf::Font();

int Fonts::loadFonts()
{
	if (!mainFont.loadFromFile("Assets/Fonts/EightBitDragon-anqx.ttf")) {
		return -1;
	}

	mainFont.setSmooth(false);
	return 0;
}

ShadedText::ShadedText(const std::string& string, const sf::Font& font, const sf::Color foregroundColor, const sf::Color backgroundColor, int textSize)
{
	foreground = new sf::Text(string, font, textSize);
	setForegroundColor(foregroundColor);

	background = new sf::Text(string, font, textSize);
	setBackgroundColor(backgroundColor);
}

void ShadedText::setString(const std::string& string) const
{
	foreground->setString(string);
	background->setString(string);
}

void ShadedText::setPosition(sf::Vector2f position)
{
	foreground->setPosition(position);
	background->setPosition(position + offset);
}

void ShadedText::setForegroundColor(sf::Color color)
{
	foreground->setFillColor(color);
}

void ShadedText::setBackgroundColor(sf::Color color)
{
	background->setFillColor(color);
}

sf::FloatRect ShadedText::getLocalBounds()
{
	return foreground->getLocalBounds();
}

void ShadedText::draw(sf::RenderTarget& target, const sf::RenderStates& states) const
{
	target.draw(*background);
	target.draw(*foreground);
}
