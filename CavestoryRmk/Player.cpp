#include "Player.h"
#include "Graphics.h"
#include "globals.h"
#include "Slope.h"
#include "Level.h"
#include "Door.h"

#include <cmath>

Player::Player()
{
}

Player::Player(Graphics& graphics, Vector2 spawnPoint) :
AnimatedSprite(graphics, "content/sprites/MyChar.png", 0, 0, 16, 16, spawnPoint.x, spawnPoint.y, 100),
_dx(0),
_dy(0),
_facing(LEFT),
_grounded(false),
_allowJumping(true),
_onSlope(false),
_isCollidingBot(true),
_lookingDown(false),
_lookingUp(false),
_maxHealth(3),
_currentHealth(1),
_currentWeaponHeld("none"),
_isInvincible(false),
_playerDead(false)

{
	this->_currentAnimation = "IdleLeft";	// If the level is restarted, player starts with this animation so it doesn't NULL, maybe better solution?
	this->setupAnimation();

	// Instantiate default weapon 
	this->_arsenal.insert({ "none", nullptr });			// No weapon held

}

void Player::draw(Graphics& graphics)
{
	if (_currentWeaponHeld != "none") {
		this->_arsenal[this->_currentWeaponHeld]->draw(graphics, this->_x, this->_y);
	}

	AnimatedSprite::draw(graphics, this->_x, this->_y);



}

void Player::update(float elapsedTime)
{
	// If the player has invincibility frames, the player texture should blink
	if (this->_isInvincible) {
		double amount = this->_timeElapsed * 2.55;	// scale to 0-255
		SDL_SetTextureAlphaMod(this->_spriteSheet, (int)amount);

		if (this->_currentWeaponHeld != "none") {
			SDL_SetTextureAlphaMod(this->_arsenal[this->_currentWeaponHeld]->getTexture(), (int)amount);
		}


	}


	if (_currentWeaponHeld != "none") {
		if (this->_lookingUp == true) {
			this->_arsenal[this->_currentWeaponHeld]->playAnimation(this->_facing == RIGHT ? "IdleAimUpRight" : "IdleAimUpLeft");
		}
		else {
			this->_arsenal[this->_currentWeaponHeld]->playAnimation(this->_facing == RIGHT ? "IdleAimRight" : "IdleAimLeft");
		}
		this->_arsenal[this->_currentWeaponHeld]->update(elapsedTime);
	}


	// Apply gravity
	if (this->_dy <= player_constants::GRAVITY_CAP) {
		this->_dy += player_constants::GRAVITY * elapsedTime;
	}
	// Move by dy
	this->_y += this->_dy * elapsedTime;

	// Move by dx
	this->_x += this->_dx * elapsedTime;
	//std::cout << this->_x << std::endl;

	AnimatedSprite::update(elapsedTime);
}

void Player::animationDone(std::string currentAnimation)
{
}


void Player::jump()
{
	if (this->_grounded && this->_allowJumping) {
		this->_dy = 0;
		this->_dy -= player_constants::JUMP_SPEED;		// _dy is negative, now for a short period (_dy / GRAVITY * timeElapsed) the character is moving upwards 
		this->_grounded = false;						// Gravity is adding positive values to _dy to make it go back down eventually
		this->_onSlope = false;
	}
}

void Player::moveLeft() {
	if (this->_lookingDown && this->_grounded) {
		return;
	}
	this->_dx = -player_constants::WALK_SPEED;
	if (this->_lookingUp == false) {
		this->playAnimation("RunLeft");
	}

	this->_facing = LEFT;
}

void Player::moveRight() {
	if (this->_lookingDown && this->_grounded) {
		return;
	}
	this->_dx = player_constants::WALK_SPEED;
	if (this->_lookingUp == false) {
		this->playAnimation("RunRight");
	}
	this->_facing = RIGHT;
}

void Player::stopMoving() {
	this->_dx = 0.0f;
	if (this->_lookingUp == false && this->_lookingDown == false) {
		this->playAnimation(this->_facing == RIGHT ? "IdleRight" : "IdleLeft");
	}

}

void Player::lookUp()
{
	this->_lookingUp = true;
	if (this->_dx == 0) {
		this->playAnimation(this->_facing == RIGHT ? "IdleRightUp" : "IdleLeftUp");
	}
	else {
		this->playAnimation(this->_facing == RIGHT ? "RunRightUp" : "RunLeftUp");
	}
}

void Player::stopLookingUp()
{
	this->_lookingUp = false;
}

void Player::lookDown()
{
	this->_lookingDown = true;
	if (this->_grounded == true) {
		//WE need to interact (turn backwards)
		this->playAnimation(this->_facing == RIGHT ? "LookBackwardsRight" : "LookBackwardsLeft");
	}
	else {
		this->playAnimation(this->_facing == RIGHT ? "LookDownRight" : "LookDownLeft");
	}
}

void Player::stopLookingDown()
{
	this->_lookingDown = false;
}

const float Player::getX() const
{
	return _x;
}

const float Player::getY() const
{
	return _y;
}

void Player::setupAnimation()
{
	this->addAnimation(3, 0, 0, "RunLeft", 16, 16, Vector2(0, 0));
	this->addAnimation(3, 0, 16, "RunRight", 16, 16, Vector2(0, 0));
	this->addAnimation(1, 0, 0, "IdleLeft", 16, 16, Vector2(0, 0));
	this->addAnimation(1, 0, 16, "IdleRight", 16, 16, Vector2(0, 0));

	this->addAnimation(1, 3, 0, "IdleLeftUp", 16, 16, Vector2(0, 0));
	this->addAnimation(1, 3, 16, "IdleRightUp", 16, 16, Vector2(0, 0));
	this->addAnimation(3, 3, 0, "RunLeftUp", 16, 16, Vector2(0, 0));
	this->addAnimation(3, 3, 16, "RunRightUp", 16, 16, Vector2(0, 0));
	this->addAnimation(1, 6, 0, "LookDownLeft", 16, 16, Vector2(0, 0));
	this->addAnimation(1, 6, 16, "LookDownRight", 16, 16, Vector2(0, 0));
	this->addAnimation(1, 7, 0, "LookBackwardsLeft", 16, 16, Vector2(0, 0));
	this->addAnimation(1, 7, 16, "LookBackwardsRight", 16, 16, Vector2(0, 0));

	// Death 
	this->addAnimation(1, 6, 28, "TombstoneIdle", 30, 28, Vector2(0, 0));
}

void Player::handleSlopeCollisions(std::vector<Slope>& others)
{
	for (int i = 0; i < others.size(); i++) {
		//Calculate where on the slope the player's bottom center is touching
		//and use y=mx+b to figure out the y position to place him at
		//First calculate "b" (slope intercept) using one of the points (b = y - mx)
		float slope = others.at(i).getSlope();
		int b = (others.at(i).getP1().y - (slope * others.at(i).getP1().x));

		if (fabs(slope) > environment::maxSlope) {
			this->_dx = 0;
			this->_x -= this->_facing == RIGHT ? 0.5f : -0.5f;
		}


		//Now get player's center x
		int centerX = this->_boundingBox.getCenterX();

		//Now pass that X into the equation y = mx + b (using our newly found b and x) to get the new y position
		int newY = (slope * centerX) + b - 6; //8 is temporary to fix a problem
		//std::cout << "colliding with " << others.size() << " slopes, " << newY << std::endl;

		//Re-position the player to the correct "y"
		if (this->_grounded) {
			//PRINTVAR("adjusting player on slope");
			this->_y = newY - this->_boundingBox.getHeight();
			this->_grounded = true;
		}

		if ((slope < 0 && this->_facing == LEFT) ||		//  This tries to prevent glitching through slopes by jumping. Jumping is disallowed when on a slope and colliding TOP
			(slope > 0 && this->_facing == RIGHT)) {	//  When the user walks down the slope again jumping is allowed again
			this->_allowJumping = true;
		}

		this->_onSlope = true;
	}
}

void Player::handleTileCollisions(std::vector<Rectangle>& others)
{
	// Figure out what side the collision happened on and move the player accordingly
	for (int i = 0; i < others.size(); i++) {
		sides::Side collisionSide = Sprite::getCollisionSide(others.at(i));
		if (collisionSide != sides::NONE) {
			switch (collisionSide) {
			case sides::TOP:						// Top of player bounding box
				this->_dy = 0;
				this->_y = others.at(i).getBottom() + 1;
				if (this->_grounded && this->_onSlope) {
					this->_dx = 0;					// stop movement if hitting TOP wall on a slope								
					this->_x -= this->_facing == RIGHT ? 1.0f : -1.0f;
					this->_allowJumping = false;
				}
				//PRINTVAR(" Collision TOP");
				break;
			case sides::BOTTOM:
				this->_y = others.at(i).getTop() - this->_boundingBox.getHeight() - 1;
				this->_dy = 0;						// This resets the velocity in y-axis, so when you fall again you fall slowly
				this->_grounded = true;
				this->_allowJumping = true;
				this->_onSlope = false;
				this->_isCollidingBot = true;
				//PRINTVAR(" Collision Bot");
				break;
			case sides::LEFT:
				this->_x = others.at(i).getRight() + 1;
				this->_allowJumping = true;
				//PRINTVAR(" Collision LEFT");
				break;
			case sides::RIGHT:
				this->_x = others.at(i).getLeft() - this->_boundingBox.getWidth() - 1;
				this->_allowJumping = true;
				//PRINTVAR(" Collision RIGHT");
				break;
			}
		}
	}
}

void Player::handleDoorCollision(std::vector<Door>& others, Level& level, Graphics& graphics)
{
	// Check if the player is grounded and holding the Down arrow
	// If so, go through the door
	// If not, do nothing
	for (int i = 0; i < others.size(); i++) {
		if (this->_grounded && this->_lookingDown) {
			level = Level(others[i].getDestination(), graphics);
			this->_x = level.getPlayerSpawnPoint().x;
			this->_y = level.getPlayerSpawnPoint().y;
		}
	}
}

void Player::handleItemCollisions(std::vector<std::shared_ptr<Item>>& others)
{
	for (int i = 0; i < others.size(); i++) {
		others[i]->touchPlayer(this, others);
	}
}

void Player::handleEnemyCollisions(std::vector<std::shared_ptr<Enemy>>& others)
{
	for (int i = 0; i < others.size(); i++) {
		others[i]->touchPlayer(this);
	}

}

void Player::gainHealth(int amount)
{
	//printf("lose %d health, current health: %d\n", amount, _currentHealth);
	this->_currentHealth += amount;
	if (this->_currentHealth <= 0) {
		this->_playerDead = true;
	}

}

void Player::addWeapon(const std::string name, std::shared_ptr<Weapon> weapon)
{
	this->_arsenal.insert({ name, weapon });
	//std::cout << this->_arsenal.size() << std::endl;
}

void Player::changeWeapon(const std::string newWeapon)
{
	if (this->_arsenal.count(newWeapon) > 0) {
		this->_currentWeaponHeld = newWeapon;
	}

}

void Player::shootWeapon()
{
	if (this->_currentWeaponHeld != "none") {
		this->_arsenal[this->_currentWeaponHeld]->fire(this->_facing, this->_lookingUp, this->_lookingDown);
	}

}

void Player::gainInvincibility(int ms)
{
	this->_isInvincible = true;
	SDL_AddTimer(ms, &setInvincibility, this);	// Periodic timer, pass this pointer to CB function to manipulate state of player object 
}

Uint32 Player::setInvincibility(Uint32 interval, void *param)
{
	// Static function, gets a player object that can be dereferenced 
	Player *player = ((Player *)param);

	player->_isInvincible = false;
	SDL_SetTextureAlphaMod(player->_spriteSheet, 255);
	if (player->_currentWeaponHeld != "none") {
		SDL_SetTextureAlphaMod(player->_arsenal[player->_currentWeaponHeld]->getTexture(), 255);
	}


	return 0;	// return 0 if the timer shouldn't reset and call the CB function again after interval has passed
}


Player::~Player()
{
}
