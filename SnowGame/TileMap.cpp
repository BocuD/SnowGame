#include "TileMap.h"

bool TileMap::load(sf::Texture* tileset, sf::Vector2u tileSize, std::vector<ldtk::Tile> tiles, unsigned width, unsigned height)
{
	texture = tileset;
	// resize the vertex array to fit the level size
	vertices.setPrimitiveType(sf::Quads);
	vertices.resize(width * height * 4);

	// populate the vertex array, with one quad per tile
	for (unsigned i = 0; i < width; ++i) 
	{
		for (unsigned j = 0; j < height; ++j)
		{
			const ldtk::Tile tile = tiles[i + j * width];

			// get the current tile number
			const int tileNumber = tile.tileId;

#pragma warning(push)
#pragma warning(disable : 4244)

			// find its position in the tileset texture
			const float tu = tileNumber % (tileset->getSize().x / tileSize.x);
			const float tv = tileNumber / (tileset->getSize().x / tileSize.x);

			// get a pointer to the current tile's quad
			sf::Vertex* quad = &vertices[(i + j * width) * 4];

			// define its 4 corners
			quad[0].position = sf::Vector2f(i * tileSize.x, j * tileSize.y);
			quad[1].position = sf::Vector2f((i + 1) * tileSize.x, j * tileSize.y);
			quad[2].position = sf::Vector2f((i + 1) * tileSize.x, (j + 1) * tileSize.y);
			quad[3].position = sf::Vector2f(i * tileSize.x, (j + 1) * tileSize.y);

			// define its 4 texture coordinates
			quad[0].texCoords = sf::Vector2f(tu * tileSize.x, tv * tileSize.y) + sf::Vector2f(0.5f, 0.5f);
			quad[1].texCoords = sf::Vector2f((tu + 1) * tileSize.x, tv * tileSize.y) + sf::Vector2f(-0.5f, 0.5f);
			quad[2].texCoords = sf::Vector2f((tu + 1) * tileSize.x, (tv + 1) * tileSize.y) + sf::Vector2f(-0.5f, -0.5f);
			quad[3].texCoords = sf::Vector2f(tu * tileSize.x, (tv + 1) * tileSize.y) + sf::Vector2f(0.5f, -0.5f);

#pragma warning(pop)

			if (tile.flipX)
			{
				sf::Vector2f temp = quad[0].texCoords;
				quad[0].texCoords = quad[1].texCoords;
				quad[1].texCoords = temp;
				temp = quad[2].texCoords;
				quad[2].texCoords = quad[3].texCoords;
				quad[3].texCoords = temp;
			}

			if (tile.flipY)
			{
				sf::Vector2f temp = quad[0].texCoords;
				quad[0].texCoords = quad[3].texCoords;
				quad[3].texCoords = temp;
				temp = quad[1].texCoords;
				quad[1].texCoords = quad[2].texCoords;
				quad[2].texCoords = temp;
			}
		}
	}
	return true;
}

void TileMap::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	// apply the transform
	states.transform *= getTransform();

	// apply the tileset texture
	states.texture = texture;

	// draw the vertex array
	target.draw(vertices, states);
}