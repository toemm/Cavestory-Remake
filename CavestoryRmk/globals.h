#pragma once

#include <SDL.h>
#include <iostream>
#include <map>
#include <string>

#define PRINTVAR(x) std::cout << x << std::endl;

// Global variables have external linkage, thus they can only be defined ONCE
// If ItemDescription wasn't static, multiple files (.c files) that include the globals.h header would include
// multiple definitions into external scope which violates One Definition Rule (one definition per scope)
// Alternativ solution: Create an anonymous namespace which creates internal linkage
// Or define it as static to remove external linkage
static std::map<std::string, std::string> ItemDescriptions = {
	{ "pistol", "pistol" },
	{ "save", "Savedisk" },
	{ "healthpack", "Healthpack" },
	{ "rifle", "Rifle" },
	{ "rocketlauncher", "Rocketlauncher" }
};

namespace bullet_physics {
	const float xVelocity = 0.8f;		// 0.3
	const float yVelocity = 0.8f;		// 0.3
	const float maxDistance = 80.0f;

}

namespace player_constants {
	const int PLAYER_HEIGHT = 32;
	const int PLAYER_WIDTH = 32;
	const float WALK_SPEED = 0.2f;
	const float GRAVITY = 0.002f;
	const float GRAVITY_CAP = 0.8f;
	const float JUMP_SPEED = 0.7f;
}

// const objects are internally linked by default
// const extern makes them externally linkable
namespace globals {
	const int SCREEN_WIDTH = 640;		// 640
	const int SCREEN_HEIGHT = 512;		// 512
	const int LEVEL_WIDTH = 1920;
	const int LEVEL_HEIGHT = 1536;
	const float SPRITE_SCALE = 2.0f;
}

namespace environment {
	const float maxSlope = 2.0f;

}

namespace audio {
	const int MUSIC_VOLUME = 7;
	const int WEAPON_VOLUME = 5;
}

namespace text {
	const int FONT_SIZE = 28;
	const SDL_Color green = { 255, 165, 0};
}

namespace sides {
	enum Side {
		TOP,
		BOTTOM,
		LEFT,
		RIGHT,
		NONE
	};

	const inline Side getOppositeSide(Side side) {
		return
			side == TOP ? BOTTOM :
			side == BOTTOM ? TOP :
			side == LEFT ? RIGHT :
			side == RIGHT ? LEFT :
			NONE;
	}
}

enum Direction {
	LEFT,
	RIGHT,
	UP,
	DOWN
};

struct Vector2 {
	int x, y;
	Vector2(int x = 0, int y = 0) :
		x(x), y(y) {}

	static Vector2 zero() {
		return Vector2();
	}
};



