#pragma once
#include <SFML/System/Vector2.hpp>
#include <complex>

class VectorUtilities
{
public:
	static sf::Vector2f normalizeVector(const sf::Vector2f input)
	{
		const float length = vectorLength(input);
		return {input.x / length, input.y / length};
	}

	static float vectorLength(const sf::Vector2f input)
	{
		return sqrt(input.x * input.x + input.y * input.y);
	}
};
