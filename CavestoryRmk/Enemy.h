#pragma once

#include "AnimatedSprite.h"
#include "globals.h"

#include <string>

class Graphics;
class Player;

class Enemy :
	public AnimatedSprite
{
public:
	Enemy();
	Enemy(Graphics& graphics, std::string filePath, int sourceX, int sourceY, int width, int height,
		Vector2 spawnPoint, int timeToUpdate);

	virtual void update(int elapsedTime, Player& player);
	virtual void draw(Graphics& graphics);

	virtual void touchPlayer(Player *player) = 0;	// Pure virtual

	//virtual void animationDone(std::string currentAnimation) override;		// Enemy objects aren't initialized, so this doesn't have to be implemented
	//virtual void setupAnimation() override;

	inline void gainHealth(int amount) { this->_currentHealth += amount; }
	const inline int getMaxHealth() const { return this->_maxHealth; }
	const inline int getCurrentHealth() const { return this->_currentHealth; }


protected:
	Direction _direction;
	float _dx, _dy;

	int _maxHealth;
	int _currentHealth;


private:

};


class Bat :
	public Enemy
{
public:
	Bat();
	Bat(Graphics& graphics, Vector2 spawnPoint);

	virtual void update(int elapsedTime, Player& player) override;
	virtual void draw(Graphics& graphics) override;

	// Pure virtual functs from AnimSprite
	virtual void animationDone(std::string currentAnimation) override;
	virtual void setupAnimation() override;

	// Pure virtual functs from Enemy
	virtual void touchPlayer(Player *player) override;


private:
	float _startingX, _startingY;	// Spawnpoint (x,y) which don't change
	bool _shouldMoveUp;



};


class Trump :
	public Enemy
{
public:
	Trump();
	Trump(Graphics& graphics, Vector2 spawnPoint);

	virtual void update(int elapsedTime, Player& player) override;
	virtual void draw(Graphics& graphics) override;

	// Pure virtual functs from AnimSprite
	virtual void animationDone(std::string currentAnimation) override;
	virtual void setupAnimation() override;

	// Pure virtual functs from Enemy
	virtual void touchPlayer(Player *player) override;


private:
	float _startingX, _startingY;	// Spawnpoint (x,y) which don't change
	bool _shouldMoveUp;




};

