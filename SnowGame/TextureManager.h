#pragma once
#include <map>
#include <string>
#include <SFML/Graphics/Texture.hpp>

class TextureManager
{
public:
	TextureManager();
	static sf::Texture* getTexture(std::string filepath);

private:
	sf::Texture* addTexture(std::string filepath);
	std::map<std::string, sf::Texture> textureCache;
	static TextureManager* instance;
};
