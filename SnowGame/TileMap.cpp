#include "TileMap.h"
#include <SFML/Graphics/Texture.hpp>

bool TileMap::load(sf::Texture* tileset, sf::Vector2u tileSize, std::vector<ldtk::Tile> tiles, unsigned width, unsigned height)
{
	texture = tileset;
	// resize the vertex array to fit the level size
	vertices.setPrimitiveType(sf::Triangles);

	vertices.resize(width * height * 6);

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
			sf::Vertex* tris = &vertices[(i + j * width) * 6];

			// define corners
			tris[0].position = { (float) i * tileSize.x, (float) j * tileSize.y };
			tris[1].position = { (float) (i + 1) * tileSize.x, (float) j * tileSize.y };
			tris[2].position = { (float) (i + 1) * tileSize.x, (float) (j + 1) * tileSize.y };

			tris[3].position = tris[0].position;
			tris[4].position = { (float) i * tileSize.x, (float)(j + 1) * tileSize.y };
			tris[5].position = tris[2].position;

			//define base texture coordinates
			tris[0].texCoords = sf::Vector2f(tu * tileSize.x, tv * tileSize.y) + sf::Vector2f(0.5f, 0.5f);
			tris[1].texCoords = sf::Vector2f((tu + 1) * tileSize.x, tv * tileSize.y) + sf::Vector2f(-0.5f, 0.5f);
			tris[2].texCoords = sf::Vector2f((tu + 1) * tileSize.x, (tv + 1) * tileSize.y) + sf::Vector2f(-0.5f, -0.5f);
			tris[3].texCoords = tris[0].texCoords;
			tris[4].texCoords = sf::Vector2f(tu * tileSize.x, (tv + 1) * tileSize.y) + sf::Vector2f(0.5f, -0.5f);
			tris[5].texCoords = tris[2].texCoords;

#pragma warning(pop)
			//see drawing for context (project root / texture coords.png)
			if (tile.flipX)
			{
				tris[0].texCoords = tris[1].texCoords;
				tris[1].texCoords = tris[3].texCoords;
				tris[3].texCoords = tris[0].texCoords;

				tris[2].texCoords = tris[4].texCoords;
				tris[4].texCoords = tris[5].texCoords;
				tris[5].texCoords = tris[2].texCoords;
			}

			if (tile.flipY)
			{
				tris[0] = tris[4];
				tris[4] = tris[3];
				tris[3] = tris[0];

				tris[5] = tris[1];
				tris[1] = tris[2];
				tris[2] = tris[5];
			}
		}
	}
	return true;
}

void TileMap::draw(sf::RenderTarget& target, const sf::RenderStates& states) const
{
	sf::RenderStates statesCopy(states);

	// apply the transform
	statesCopy.transform *= getTransform();

	// apply the tileset texture
	statesCopy.texture = texture;

	// draw the vertex array
	target.draw(vertices, statesCopy);
}
