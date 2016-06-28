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

	// Load the surface from the graphics object, transform and store into a SDL_Texture
	SDL_Surface *surf = graphics.loadText(this->_textdata);

	// If update() is not called, x and y coordinates remain static
	this->_sourceRect = { this->_x, this->_y, surf->w, surf->h };

	this->_textTexture = SDL_CreateTextureFromSurface(graphics.getRenderer(), surf);
	if (this->_textTexture == NULL) {
		printf("\nERROR: Unable to load text image, %s\n", SDL_GetError());
	}

	// The surface isn't needed anymore because the texture has been created and the pointer stored in this object
	SDL_FreeSurface(surf);
}

void Text::update()
{
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