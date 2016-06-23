#include <SDL.h>
#include <SDL_image.h>

#include "Graphics.h"
#include "globals.h"
#include "Camera.h"

Graphics::Graphics()
{
	SDL_CreateWindowAndRenderer(globals::SCREEN_WIDTH, globals::SCREEN_HEIGHT, SDL_WINDOW_BORDERLESS, &this->_window, &this->_renderer);
	SDL_SetWindowTitle(this->_window, "Cavestory Remake");

	// GREEN LINES for Debugging
	SDL_SetRenderDrawColor(this->_renderer, 0, 255, 0, 255);
}


Graphics::~Graphics()
{
	SDL_DestroyWindow(this->_window);
	SDL_DestroyRenderer(this->_renderer);

}

SDL_Surface *Graphics::loadImage(const std::string& filePath)
{
	if (this->_spriteSheets.count(filePath) == 0) {
		this->_spriteSheets[filePath] = IMG_Load(filePath.c_str());
	}
	return this->_spriteSheets[filePath];
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