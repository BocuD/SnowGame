#pragma once
#include <LDtkLoader/Tile.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/VertexArray.hpp>

namespace ldtk
{
	class Tile;
}

class TileMap : public sf::Drawable, public sf::Transformable
{
public:
    bool load(sf::Texture* tileset, sf::Vector2u tileSize, std::vector<ldtk::Tile> tiles, unsigned width, unsigned height);

private:
	void draw(sf::RenderTarget& target, const sf::RenderStates& states) const override;

    sf::Texture* texture;
    sf::VertexArray vertices;
};
