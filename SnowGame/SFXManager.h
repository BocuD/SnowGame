#pragma once
#include <map>
#include <string>
#include <SFML/Audio/SoundBuffer.hpp>

class SFXManager
{
public:
	SFXManager();
	static sf::SoundBuffer* getSoundBuffer(std::string filepath);

private:
	sf::SoundBuffer* addClip(std::string filepath);
	std::map<std::string, sf::SoundBuffer> audioCache;
	static SFXManager* instance;
};
