#pragma once

#include <SDL_ttf.h>
#include <SDL.h>
#include <string>

class Graphics;

struct TextData {
	std::string text;
	TTF_Font *font;
	SDL_Color color;

	bool operator< (const TextData& other) const {
		return this->text < other.text;
	}

};

class Text
{
public:
	Text();
	Text(Graphics& graphics, const std::string& text, int posX, int posY,
		const std::string& fontFilePath, SDL_Color col);
	~Text();

	void update();
	void draw(Graphics& graphics, int x, int y);

	const inline TextData getTextData() const { return this->_textdata; }
	const inline int getX() const { return this->_x; }
	const inline int getY() const { return this->_y; }

private:
	int _x, _y;

	TextData _textdata;
	SDL_Texture *_textTexture;
	SDL_Rect _sourceRect;
};

