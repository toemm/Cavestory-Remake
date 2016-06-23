#pragma once

#include "Sprite.h"
#include "Player.h"

class Graphics;

class Hud
{
public:
	Hud();
	Hud(Graphics& graphics);

	void update(int elapsedTime, Player& player);
	void draw(Graphics& graphics);

	~Hud();

private:
			
	//Health sprites
	Sprite _healthBarSprite;
	Sprite _healthNumber1;
	Sprite _currentHealthBar;

	//EXP sprite
	Sprite _lvWord;
	Sprite _lvNumber;
	Sprite _expBar;

	//Weapon info
	Sprite _slash;
	Sprite _dashes;

};

