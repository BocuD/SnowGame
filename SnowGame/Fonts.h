#pragma once
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>

class Fonts
{
public:
	static sf::Font mainFont;

	static int loadFonts();
};

class ShadedText : public sf::Drawable, public sf::Transformable
{
public:
	sf::Vector2f offset = {3, 3};

	ShadedText(const std::string& string, const sf::Font& font, sf::Color foregroundColor, sf::Color backgroundColor, int textSize);
	void setString(const std::string& string) const;
	void setPosition(sf::Vector2f position);
	void setForegroundColor(sf::Color color);
	void setBackgroundColor(sf::Color color);

	sf::FloatRect getLocalBounds();
	void draw(sf::RenderTarget& target, const sf::RenderStates& states) const override;

private:
	sf::Text* foreground;
	sf::Text* background;
};