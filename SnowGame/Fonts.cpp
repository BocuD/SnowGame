#include "Fonts.h"

sf::Font Fonts::mainFont = sf::Font();

int Fonts::loadFonts()
{
	if (!mainFont.loadFromFile("Assets/Fonts/EightBitDragon-anqx.ttf")) {
		return -1;
	}
	return 0;
}