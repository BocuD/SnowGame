#include "TextureManager.h"

#include <iostream>

TextureManager* TextureManager::instance = nullptr;

TextureManager::TextureManager()
{
	if(instance != nullptr)
	{
		std::cout << "tried constructing second singleton instance" << std::endl;
	}
	instance = this;
}

sf::Texture* TextureManager::getTexture(std::string filepath)
{
	if(instance->textureCache.find(filepath) != instance->textureCache.end())
	{
		return &instance->textureCache[filepath];
	}
	else
	{
		return instance->addTexture(filepath);
	}
}

sf::Texture* TextureManager::addTexture(std::string filepath)
{
	sf::Texture newTexture;
	newTexture.loadFromFile(filepath);
	textureCache[filepath] = newTexture;
	return &textureCache[filepath];
}