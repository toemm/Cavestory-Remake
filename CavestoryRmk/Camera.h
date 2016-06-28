#pragma once

#include "Player.h"
#include "globals.h"

#include <SDL.h>

class Camera {
public:
	void update(int elapsedTime, Player& player);

	// return center (x, y) coordinates
	static Vector2 getCenter();	

	// Declaration
	static SDL_Rect cameraRect;

};

