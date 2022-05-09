#include "AnimatedGameObject.h"
#include <SFML/Graphics/Texture.hpp>

void AnimatedGameObject::addTexture(std::string path, int frames)
{
	sf::Texture tex;
	tex.loadFromFile(path);
	textures.push_back(tex);
	frameCount.push_back(frames);

	if (!hasTexture) 
	{
		hasTexture = true;
		setTextureId(0);
	}
}

void AnimatedGameObject::addTexture(sf::Texture* tex, int frames)
{
	textures.push_back(*tex);
	frameCount.push_back(frames);

	if (!hasTexture) 
	{
		hasTexture = true;
		setTextureId(0);
	}
}

void AnimatedGameObject::setTextureId(int id)
{
	setTexture(textures[id]);
	setAnimationFrame(0);
	currentFrame = 0;
	textureId = id;
}

void AnimatedGameObject::setAnimationFrame(int frame)
{
	setTextureRect(sf::IntRect({ frame * textureSize.x, 0 }, { textureSize.x, textureSize.y }));
}

void AnimatedGameObject::fixedUpdate()
{
	if(animate)
	{
		delayStep++;
		if(delayStep >= delayFrames)
		{
			delayStep = 0;
			currentFrame++;
			if (currentFrame > frameCount[textureId] - 1) currentFrame = 0;

			setAnimationFrame(currentFrame);
		}
	}
}