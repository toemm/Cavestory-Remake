#include "Sprite.h"
#include "Graphics.h"
#include "globals.h"
#include "Rectangle.h"

#include <iostream>

Sprite::Sprite()
{
}


Sprite::~Sprite()
{

}

Sprite::Sprite(Graphics& graphics, const std::string& filePath, int sourceX, int sourceY, int width, int height, float posX, float posY) : 
	_x(posX),	// destX
	_y(posY)	// destY
{
	this->_sourceRect.x = sourceX;
	this->_sourceRect.y = sourceY;
	this->_sourceRect.h = height;
	this->_sourceRect.w = width;

	this->_spriteSheet = SDL_CreateTextureFromSurface(graphics.getRenderer(), graphics.loadImage(filePath));
	if (this->_spriteSheet == NULL) {
		printf("\nERROR: Unable to load image, %s\n", SDL_GetError());
	}

	this->_boundingBox = Rectangle(this->_x, this->_y, width * globals::SPRITE_SCALE, height * globals::SPRITE_SCALE);
}


 void Sprite::update()
{

	 this->_boundingBox.setX(this->_x);
	 this->_boundingBox.setY(this->_y);
	 this->_boundingBox.setW(this->_sourceRect.w * globals::SPRITE_SCALE);
	 this->_boundingBox.setH(this->_sourceRect.h * globals::SPRITE_SCALE);

}

void Sprite::draw(Graphics& graphics, int x, int y)
{

	SDL_Rect destRect = {
		x,
		y,
		this->_sourceRect.w * globals::SPRITE_SCALE,
		this->_sourceRect.h * globals::SPRITE_SCALE
	};

	graphics.blitSurface(this->_spriteSheet, &this->_sourceRect, &destRect);
}

Rectangle& Sprite::getBoundingBox()  
{
	return this->_boundingBox;
}

// Determine which side the collision happened on
const sides::Side Sprite::getCollisionSide(Rectangle& other) const 
{
	int amtRight, amtLeft, amtTop, amtBot;		// amount of overlap between 2 rects
	amtRight = this->_boundingBox.getRight() - other.getLeft();		
	amtLeft = this->_boundingBox.getLeft() - other.getRight();
	amtTop = this->_boundingBox.getTop() - other.getBottom();
	amtBot = this->_boundingBox.getBottom() - other.getTop();

	int vals[4] = { abs(amtRight), abs(amtLeft), abs(amtTop), abs(amtBot) };
	int lowest = vals[0];

	for (int i = 0; i < 4; i++) {
		if (vals[i] < lowest) {
			lowest = vals[i];
		}
	}

	//std::cout << abs(amtRight) << ", " << abs(amtLeft) << ", " << abs(amtTop) << ", " << abs(amtBot) << ", lowest: " << lowest;

	return
		lowest == abs(amtRight) ? sides::RIGHT :
		lowest == abs(amtLeft) ? sides::LEFT :
		lowest == abs(amtTop) ? sides::TOP :
		lowest == abs(amtBot) ? sides::BOTTOM :
		sides::NONE;
}

void Sprite::setSourceRectX(int value)
{
	this->_sourceRect.x = value;
}

void Sprite::setSourceRectY(int value)
{
	this->_sourceRect.y = value;
}

void Sprite::setSourceRectW(int value)
{
	this->_sourceRect.w = value;
}

void Sprite::setSourceRectH(int value)
{
	this->_sourceRect.h = value;
}