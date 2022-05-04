#pragma once
#include <string>
#include <LDtkLoader/Tile.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/Vertex.hpp>
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

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

    sf::Texture* texture;
    sf::VertexArray vertices;
};
