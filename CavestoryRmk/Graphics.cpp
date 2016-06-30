#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include "Graphics.h"
#include "globals.h"
#include "Camera.h"

Graphics::Graphics()
{
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_CreateWindowAndRenderer(globals::SCREEN_WIDTH, globals::SCREEN_HEIGHT, SDL_WINDOW_BORDERLESS, &this->_window, &this->_renderer);
	SDL_SetWindowTitle(this->_window, "Cavestory Remake");

	// GREEN LINES for Debugging
	SDL_SetRenderDrawColor(this->_renderer, 0, 255, 0, 255);

	// Initialize the image loading library SDL_image (optional)
	int flags = IMG_INIT_PNG;
	int initted = IMG_Init(flags);	// returns initted flags

	if (initted != flags) {
		printf("IMG_Init: Failed to init required jpg and png support!\n");
		printf("IMG_Init: %s\n", IMG_GetError());
		// handle error
	}

	// Initialize SDL_ttf fonts library
	if (TTF_Init() == -1) {
		printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
	}


}


Graphics::~Graphics()
{
	SDL_DestroyWindow(this->_window);
	SDL_DestroyRenderer(this->_renderer);

	// Free the surfaces that have been created
	for (const auto& pair : this->_spriteSheets) {
		SDL_DestroyTexture(pair.second);
	}
	for (const auto& pair : this->_textLibrary) {
		SDL_DestroyTexture(pair.second);
	}

	TTF_Quit();
	IMG_Quit();
}

SDL_Texture *Graphics::loadImage(const std::string& filePath)
{
	if (this->_spriteSheets.count(filePath) == 0) {
		SDL_Surface *surf = IMG_Load(filePath.c_str());
		this->_spriteSheets[filePath] = SDL_CreateTextureFromSurface(this->getRenderer(), surf);
		SDL_FreeSurface(surf);
	}
	//printf("%i\n", _spriteSheets.size());
	return this->_spriteSheets[filePath];
}

SDL_Texture *Graphics::loadText(const TextData& data)
{
	if (this->_textLibrary.count(data) == 0) {
		SDL_Surface *surf = TTF_RenderText_Solid(data.font, data.text.c_str(), data.color);
		this->_textLibrary[data] = SDL_CreateTextureFromSurface(this->getRenderer(), surf);
		SDL_FreeSurface(surf);
	}
	//printf("%i\n", _textLibrary.size());
	return this->_textLibrary[data];

}

TTF_Font *Graphics::loadFont(const std::string& filePath, int height)
{
	if (this->_fontLibrary.count(filePath) == 0) {
		this->_fontLibrary[filePath] = TTF_OpenFont(filePath.c_str(), height);		// TODO: fix fontsLibrary to work with different sizes
	}
	//printf("%i\n", _spriteSheets.size());
	return this->_fontLibrary[filePath];
}


void Graphics::blitSurface(SDL_Texture *texture, SDL_Rect *sourceRectangle, SDL_Rect *destinationRectangle)
{

	// coordinate transformation into the camera's x, y
	destinationRectangle->x -= Camera::cameraRect.x;
	destinationRectangle->y -= Camera::cameraRect.y;

	SDL_RenderCopy(this->_renderer, texture, sourceRectangle, destinationRectangle);
}

void Graphics::flip()
{
	SDL_RenderPresent(this->_renderer);
}

void Graphics::clear()
{
	SDL_RenderClear(this->_renderer);
}

SDL_Renderer *Graphics::getRenderer() const
{
	return this->_renderer;
}