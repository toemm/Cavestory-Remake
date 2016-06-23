/* Sprite class
* Holds all information for individual sprites
*/

#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include "Rectangle.h"
#include "globals.h"

#include <string>

class Graphics;

class Sprite
{
public:
	Sprite();
	Sprite(Graphics& graphics, const std::string& filePath, int sourceX, int sourceY, int width, int height,
		float posX, float posY);
	virtual ~Sprite();
	virtual void update();
	virtual void draw(Graphics& graphics, int x, int y);

	Rectangle& getBoundingBox();
	inline SDL_Texture *getTexture() const { return this->_spriteSheet; }
	const sides::Side getCollisionSide(Rectangle& other) const;

	const inline float getX() const { return this->_x; }
	const inline float getY() const { return this->_y; }

	void setSourceRectX(int value);
	void setSourceRectY(int value);
	void setSourceRectW(int value);
	void setSourceRectH(int value);


protected:
	SDL_Rect _sourceRect;
	SDL_Texture *_spriteSheet;
	float _x, _y;
	Rectangle _boundingBox;
	

private:
	
};

