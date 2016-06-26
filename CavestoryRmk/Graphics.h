/* Graphics class
* Holds all information dealing with graphics in the game
*/

#pragma once

#include <SDL.h>
#include <SDL_ttf.h>
#include "Text.h"
#include <string>
#include <map>

struct SDL_Window;
struct SDL_Renderer;

class Graphics
{
public:
	Graphics();
	~Graphics();

	/* SDL_Surface *loadImage;
	 * Loads an image into the _spriteSheets map if it doesn't already exist
	 * As as result, each image will only ever be loaded once
	 * Returns the image from the map regardless of whether or not it was just loaded
	 */
	SDL_Surface *loadImage(const std::string& filePath);
	SDL_Surface *loadText(const TextData& data);
	TTF_Font *loadFont(const std::string& filePath, int height);

	/* void blitSurface
	 * Draws a texture to a certain part of the screen
	 */
	void blitSurface(SDL_Texture *source, SDL_Rect *sourceRectangle, SDL_Rect *destinationRectangle);

	/* void flip
	 * Renders everything to the screen
	 */
	void flip();

	/* void clear
	 * Clears the screen
	 */
	void clear();

	/* SDL_Renderer *getRenderer
	 * Returns the renderer
	 */
	SDL_Renderer *getRenderer() const;

private:
	SDL_Window *_window;
	SDL_Renderer *_renderer;

	std::map<std::string, SDL_Surface *> _spriteSheets;
	std::map<TextData, SDL_Surface *> _textLibrary;
	std::map<std::string, TTF_Font *> _fontLibrary;
};
