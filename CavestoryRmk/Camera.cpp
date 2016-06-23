#include "Camera.h"

#include "globals.h"

// Definition
SDL_Rect Camera::cameraRect;

void Camera::update(int elapsedTime, Player& player)
{

	// Get the player coordinates as base coordinates for the camera
	this->cameraRect.x = player.getX();
	this->cameraRect.y = player.getY();

	// Center the camera over the player
	// cameraRect.x, cameraRect.y contains player's x, y coordinates
	// Then add the player width and the screenWidth / 2 to form the camera x and y position
	// Then every Rectangle has to be transformed onto these camera coordinates

	// These are the definite x,y positions of the camera
	// cameraRext contains the current player position's x,y and is increased so the screen is focussed on the player
	int camX = this->cameraRect.x  - globals::SCREEN_WIDTH / 2;
	int camY = this->cameraRect.y  - globals::SCREEN_HEIGHT / 2;

	//// [X][X][X]    The following logic keeps the camera's x, y in bounds
	//// [X][ ][X]	If the player is in the X's position the camera is kept so it doesn't clip
	//// [X][X][X]	to the outside
	if (camX < 0) {
		camX = 0;
	}
	if (camY < 0) {
		camY = 0;
	}
	if (camX > globals::LEVEL_WIDTH - globals::SCREEN_WIDTH) {
		camX = globals::LEVEL_WIDTH - globals::SCREEN_WIDTH;
	}
	if (camY > globals::LEVEL_HEIGHT - globals::SCREEN_HEIGHT) {
		camY = globals::LEVEL_HEIGHT - globals::SCREEN_HEIGHT;
	}

	// TODO: Check why you can't use the static variable throughout...
	this->cameraRect.x = camX;
	this->cameraRect.y = camY;

}