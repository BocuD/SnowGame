#include "SFXManager.h"

#include <iostream>

SFXManager* SFXManager::instance = nullptr;

SFXManager::SFXManager()
{
	if(instance != nullptr)
	{
		std::cout << "tried constructing second singleton instance" << std::endl;
	}
	instance = this;
}

sf::SoundBuffer* SFXManager::getSoundBuffer(std::string filepath)
{
	if(instance->audioCache.find(filepath) != instance->audioCache.end())
	{
		return &instance->audioCache[filepath];
	}
	else
	{
		return instance->addClip(filepath);
	}
}

sf::SoundBuffer* SFXManager::addClip(std::string filepath)
{
	sf::SoundBuffer newBuffer;

	if(!newBuffer.loadFromFile(filepath))
	{
		std::cout << "Failed to load texture " << filepath << std::endl;

		return nullptr;
	}
	else
	{
		audioCache[filepath] = newBuffer;
	}

	return &audioCache[filepath];
}