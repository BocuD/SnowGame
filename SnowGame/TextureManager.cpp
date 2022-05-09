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

	if(!newTexture.loadFromFile(filepath))
	{
		std::cout << "Failed to load texture " << filepath << std::endl;
		sf::Texture errorTex;

		if (errorTex.create(16, 16))
		{
			textureCache[filepath] = errorTex;
		}
		else throw std::exception("Couldn't create fallback texture, we are probably out of memory");
	}
	else
	{
		textureCache[filepath] = newTexture;
	}

	return &textureCache[filepath];
}