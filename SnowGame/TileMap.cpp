#include "TileMap.h"

bool TileMap::load(const std::string& tileset, sf::Vector2u tileSize, std::vector<ldtk::Tile> tiles, unsigned width, unsigned height)
{
	// load the tileset texture
	if (!m_tileset.loadFromFile(tileset))
		return false;

	// resize the vertex array to fit the level size
	m_vertices.setPrimitiveType(sf::Quads);
	m_vertices.resize(width * height * 4);

	// populate the vertex array, with one quad per tile
	for (unsigned i = 0; i < width; ++i) 
	{
		for (unsigned j = 0; j < height; ++j)
		{
			const ldtk::Tile tile = tiles[i + j * width];

			// get the current tile number
			const int tileNumber = tile.tileId;

			// find its position in the tileset texture
			const float tu = tileNumber % (m_tileset.getSize().x / tileSize.x);
			const float tv = tileNumber / (m_tileset.getSize().x / tileSize.x);

			// get a pointer to the current tile's quad
			sf::Vertex* quad = &m_vertices[(i + j * width) * 4];

			// define its 4 corners
			quad[0].position = sf::Vector2f(i * tileSize.x, j * tileSize.y);
			quad[1].position = sf::Vector2f((i + 1) * tileSize.x, j * tileSize.y);
			quad[2].position = sf::Vector2f((i + 1) * tileSize.x, (j + 1) * tileSize.y);
			quad[3].position = sf::Vector2f(i * tileSize.x, (j + 1) * tileSize.y);

			// define its 4 texture coordinates
			quad[0].texCoords = sf::Vector2f(tu * tileSize.x, tv * tileSize.y);
			quad[1].texCoords = sf::Vector2f((tu + 1) * tileSize.x, tv * tileSize.y);
			quad[2].texCoords = sf::Vector2f((tu + 1) * tileSize.x, (tv + 1) * tileSize.y);
			quad[3].texCoords = sf::Vector2f(tu * tileSize.x, (tv + 1) * tileSize.y);

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
	states.texture = &m_tileset;

	// draw the vertex array
	target.draw(m_vertices, states);
}