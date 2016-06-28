#include "Text.h"
#include "Graphics.h"
#include "globals.h"
#include "Camera.h"


Text::Text()
{
}


Text::~Text()
{
}


Text::Text(Graphics& graphics, const std::string& text, Vector2 off, int fontSize, int posX, int posY,
	const std::string& fontFilePath, SDL_Color col) :
	_x(posX),
	_y(posY),
	_offset(off)

{
	this->_textdata = {
		text,
		graphics.loadFont(fontFilePath.c_str(), fontSize),
		col
	};

	// Load an SDL_Texture from the fontdata
	this->_textTexture = graphics.loadText(this->_textdata);

	// If update() is not called, x and y coordinates remain static
	this->_sourceRect = { this->_x, this->_y, 0, 0 };

	// Parse out width and height of the texture
	SDL_QueryTexture(this->_textTexture, NULL, NULL, &this->_sourceRect.w, &this->_sourceRect.h);


	if (this->_textTexture == NULL) {
		printf("\nERROR: Unable to load text image, %s\n", SDL_GetError());
	}


}

void Text::update()
{
	// Basepoint for text drawing is in the middle of the visible screen
	this->_x = Camera::getCenter().x + this->_offset.x;
	this->_y = Camera::getCenter().y + this->_offset.y;
}

void Text::draw(Graphics& graphics)
{
	SDL_Rect destRect = {
		this->_x,
		this->_y,
		this->_sourceRect.w * globals::SPRITE_SCALE,
		this->_sourceRect.h * globals::SPRITE_SCALE
	};

	graphics.blitSurface(this->_textTexture, &this->_sourceRect, &destRect);
}