#pragma once

#include "globals.h"

struct SDL_Texture;
class Graphics;

class Tile
{
public:
	Tile();
	Tile(SDL_Texture *tileset, Vector2 size, Vector2 tilesetPosition, Vector2 position);

	virtual void update(int elapsedTime);
	virtual void draw(Graphics& graphics);

	virtual ~Tile() = default;

protected:
	Vector2 _position;
	Vector2 _size;
	SDL_Texture *_tileset;
	Vector2 _tilesetPosition;
};

