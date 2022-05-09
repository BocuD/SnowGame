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

	static float distance(const sf::Vector2f A, const sf::Vector2f B)
	{
		return vectorLength(A - B);
	}

	static sf::Vector2f lerp(const sf::Vector2f A, const sf::Vector2f B, const float t)
	{
		return B * t + A * (1.f - t);
	}
};
