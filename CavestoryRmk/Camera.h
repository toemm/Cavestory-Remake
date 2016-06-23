#pragma once

#include "Player.h"
#include <SDL.h>

class Camera {
public:
	void update(int elapsedTime, Player& player);

	// Declaration
	static SDL_Rect cameraRect;

};

