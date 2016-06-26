#include "Text.h"
#include "Graphics.h"
#include "globals.h"


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

	this->_sourceRect = {
		0,
		0,
		surf->w,
		surf->h
	};

	this->_textTexture = SDL_CreateTextureFromSurface(graphics.getRenderer(), surf);
	if (this->_textTexture == NULL) {
		printf("\nERROR: Unable to load text image, %s\n", SDL_GetError());
	}
}

void Text::update()
{

}

void Text::draw(Graphics& graphics, int x, int y)
{
	SDL_Rect destRect = {
		x,
		y,
		this->_sourceRect.w * globals::SPRITE_SCALE,
		this->_sourceRect.h * globals::SPRITE_SCALE
	};

	graphics.blitSurface(this->_textTexture, &this->_sourceRect, &destRect);
}