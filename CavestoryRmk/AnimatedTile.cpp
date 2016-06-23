#include "AnimatedTile.h"
#include "Graphics.h"

#include <SDL.h>


AnimatedTile::AnimatedTile(std::vector<Vector2> tilesetPositions, int duration, SDL_Texture *tileset, Vector2 size, Vector2 position, int ID) :
Tile(tileset, size, tilesetPositions.at(0), position),
_tilesetPositions(tilesetPositions),
_duration(duration),
_tileToDraw(0),
_ID(ID),
_animate(true)
{

}

void AnimatedTile::update(int elapsedTime)
{
	// Timer code, same as updating the frameIndex in AnimatedSprote
	// Sets _tileToDraw to the right position in the _tilesetPosition array
	// _tileSetPosition contains the coordinates of the right frame in the Tileset
	if (this->_amountOfTime <= 0) {
		if (this->_tileToDraw == this->_tilesetPositions.size() - 1) {
			this->_tileToDraw = 0;
		}
		else {
			this->_tileToDraw++;
		}
		this->_amountOfTime = this->_duration;
	}
	else {
		this->_amountOfTime -= elapsedTime;
	}

	Tile::update(elapsedTime);
}

void AnimatedTile::draw(Graphics& graphics)
{
	if (this->_animate) {
		SDL_Rect destRect = {
			this->_position.x,
			this->_position.y,
			this->_size.x * globals::SPRITE_SCALE,
			this->_size.y * globals::SPRITE_SCALE };

		SDL_Rect sourceRect = {
			this->_tilesetPositions.at(this->_tileToDraw).x,
			this->_tilesetPositions.at(this->_tileToDraw).y,
			this->_size.x,
			this->_size.y };

		graphics.blitSurface(this->_tileset, &sourceRect, &destRect);
	}

}


AnimatedTile::~AnimatedTile()
{

}