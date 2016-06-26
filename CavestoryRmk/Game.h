/* Game Class
* This class holds all information for our game loop
*/

#include <iostream>
#include <fstream>

#include "Player.h"
#include "Level.h"
#include "Hud.h"
#include "Graphics.h"
#include "Input.h"
#include "Camera.h"
#include "Audio.h"
#include "Text.h"

#pragma once

class Game
{
public:
	Game();
	~Game();

private:
	void gameLoop();

	void input();
	void draw();
	void update(int elapsedTime);

	void drawDebugLines();

	Text _text;

	Audio _audio;
	SDL_Event _sdlevent;
	Input _input;
	Player _player;
	Level _level;
	Hud _hud;
	Graphics _graphics;
	Camera _camera;


	bool _running;
};

