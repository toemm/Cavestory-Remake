#include "Enemy.h"
#include "Graphics.h"
#include "Player.h"


#include <string>

// Base enemy class
Enemy::Enemy()
{
}

Enemy::Enemy(Graphics& graphics, std::string filePath, int sourceX, int sourceY, int width, int height,
	Vector2 spawnPoint, int timeToUpdate) :
	AnimatedSprite(graphics, filePath, sourceX, sourceY, width, height,
	spawnPoint.x, spawnPoint.y, timeToUpdate),
	_direction(LEFT),
	_maxHealth(3),
	_currentHealth(3),
	_dx(0.0f),
	_dy(0.0f)
	
{
	this->_boundingBox.setCollision(true);	// Usually every enemy has collision detection
}

void Enemy::update(int elapsedTime, Player& player)
{

	AnimatedSprite::update(elapsedTime);
}

void Enemy::draw(Graphics& graphics)
{
	AnimatedSprite::draw(graphics, this->_x, this->_y);
}


// Bat Class
Bat::Bat()
{

}

Bat::Bat(Graphics& graphics, Vector2 spawnPoint) :
Enemy(graphics, "content/sprites/NpcCemet.png", 32, 32, 16, 16, spawnPoint, 140),
_startingX(spawnPoint.x),
_startingY(spawnPoint.y),
_shouldMoveUp(false)
{
	this->setupAnimation();
	this->playAnimation("FlyLeft");
}


void Bat::update(int elapsedTime, Player& player)
{
	this->_direction = player.getX() > this->_x ? RIGHT : LEFT;
	this->playAnimation(this->_direction == RIGHT ? "FlyRight" : "FlyLeft");

	if (this->getCurrentHealth() <= 0) {		// Play death animation

		this->_boundingBox.setCollision(false);		// So the model that is falling down doesn't hurt the player
		this->playAnimation(this->_direction == RIGHT ? "DeathFallRight" : "DeathFallLeft");

		if (this->_dy <= player_constants::GRAVITY_CAP) {
			this->_dy += player_constants::GRAVITY;
		}

		this->_y += this->_dy * elapsedTime;
	}
	else {
		//Move up or down
		this->_y += this->_shouldMoveUp ? -.02 : .02;
		if (std::fabs(this->_startingY - this->_y) > 20) {
			this->_shouldMoveUp = !this->_shouldMoveUp;
		}
	}



	Enemy::update(elapsedTime, player);
}

void Bat::draw(Graphics& graphics)
{
	Enemy::draw(graphics);




}

void Bat::animationDone(std::string currentAnimation)
{

}

void Bat::setupAnimation()
{
	this->addAnimation(3, 2, 32, "FlyLeft", 16, 16, Vector2::zero());
	this->addAnimation(3, 2, 48, "FlyRight", 16, 16, Vector2::zero());

	//Death animation
	this->addAnimation(1, 0, 32, "DeathFallLeft", 16, 16, Vector2::zero());
	this->addAnimation(1, 0, 48, "DeathFallRight", 16, 16, Vector2::zero());

	this->addAnimation(1, 5, 32, "DeathGroundLeft", 16, 16, Vector2::zero());
	this->addAnimation(1, 5, 48, "DeathGroundRight", 16, 16, Vector2::zero());

}



void Bat::touchPlayer(Player *player)
{
	player->gainHealth(-1);
	player->gainInvincibility(3000);

}











Trump::Trump()
{

}

Trump::Trump(Graphics& graphics, Vector2 spawnPoint) :
	Enemy(graphics, "content/sprites/Trump.png", 32, 32, 40, 123, spawnPoint, 140),
	_startingX(spawnPoint.x),
	_startingY(spawnPoint.y),
	_shouldMoveUp(false)
{
	this->setupAnimation();
	this->playAnimation("Idle");

}

void Trump::update(int elapsedTime, Player& player)
{
	if (this->getCurrentHealth() <= 0) {
		this->setVisible(false);

	}
	Enemy::update(elapsedTime, player);
}

void Trump::draw(Graphics& graphics)
{
	Enemy::draw(graphics);
}

void Trump::animationDone(std::string currentAnimation)
{
}

void Trump::setupAnimation()
{
	this->addAnimation(1, 0, 0, "Idle", 79, 227, Vector2::zero());
}


void Trump::touchPlayer(Player *player)
{

}
