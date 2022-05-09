#pragma once
#include <vector>
#include "GameObject.h"

class AnimatedGameObject : public GameObject
{
public:
	std::vector<sf::Texture> textures;
	std::vector<int> frameCount;
	sf::Vector2i textureSize;

	bool animate = false;
	int currentFrame = 0;
	int delayStep = 0;
	int delayFrames = 4;
	int textureId = 0;

	void addTexture(std::string path, int frames);
	void addTexture(sf::Texture* tex, int frames);

	void setTextureId(int id);
	void setAnimationFrame(int frame);
	void fixedUpdate() override;

private:
	bool hasTexture = false;;
};