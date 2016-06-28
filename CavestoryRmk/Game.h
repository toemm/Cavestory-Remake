/* Game Class
* This class holds all information for our game loop
*/

#include <iostream>
#include <fstream>
#include <map>

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

	// Restart game, load first level
	void restart();

	// Initialiize text objects that are needed for the game object
	void initText();

	// Draw rectangles around sprites for debugging purposes
	void drawDebugLines();

	Audio _audio;
	SDL_Event _sdlevent;
	Input _input;
	Player _player;
	Level _level;
	Hud _hud;
	Graphics _graphics;
	Camera _camera;

	std::map<std::string, Text> _gameTexts;

	bool _running;
};

