#pragma once

#include "AnimatedSprite.h"
#include "globals.h"
#include "Slope.h"
#include "Enemy.h"
#include "Item.h"

#include <memory>
#include <vector>

class Door;
class Level;
class Graphics;

class Player :
	public AnimatedSprite
{
public:
	Player();
	Player(Graphics& graphics, Vector2 spawnPoint);
	virtual void draw(Graphics& graphics);
	virtual void update(float elapsedTime);
	virtual void animationDone(std::string currentAnimation) override;
	virtual void setupAnimation() override;

	void moveLeft();
	void moveRight();
	void stopMoving();
	void jump();
	void lookUp();
	void stopLookingUp();
	void lookDown();
	void stopLookingDown();

	const float getX() const;
	const float getY() const;

	/* void handleTileCollisions
	* Handles collisions with ALL tiles the player is colliding with
	*/
	void handleTileCollisions(std::vector<Rectangle>& others);

	/* void handleSlopeCollisions
	* Handles collisions with the player and slopes in the level
	*/
	void handleSlopeCollisions(std::vector<Slope>& others);

	/* void handleDoorCollision
	* Handles collision with doors in a level
	*/
	void handleDoorCollision(std::vector<Door>& others, Level& level, Graphics& graphics);

	/* void handlEnemyCollision
	* Handles collision with enemy objects (bats IS AN enemy, etc..)
	*/
	void handleEnemyCollisions(std::vector<std::shared_ptr<Enemy>>& others);

	/* void handlEnemyCollision
	* Handles collision with items (savedisk, weapons, healthpacks, etc..)
	*/
	void handleItemCollisions(std::vector<std::shared_ptr<Item>>& others);


	const inline bool isGrounded() const { return this->_grounded; }
	const inline bool isAllowedJumping() const { return this->_allowJumping; }
	const inline bool isOnSlope() const { return this->_onSlope; }
	const inline bool isCollidingBot() const { return this->_isCollidingBot; }

	const inline int getMaxHealth() const { return this->_maxHealth; }
	const inline int getCurrentHealth() const { return this->_currentHealth; }
	
	void gainHealth(int amount);

	void addWeapon(const std::string name, std::shared_ptr<Weapon> weapon);
	void changeWeapon(const std::string newWeapon);
	void shootWeapon();
	inline const std::shared_ptr<Weapon> getCurrentWeapon() const { return this->_arsenal.at(this->_currentWeaponHeld); }
	inline const std::string getWeaponHeldString() const { return this->_currentWeaponHeld; }

	// Invincibility frames if hit by an enemy
	void gainInvincibility(int ms);
	static Uint32 setInvincibility(Uint32 interval, void *param);				// static function, pointer to static function member = pointer to function
	inline const bool isInvincible() const { return this->_isInvincible; }

	~Player();

private:
	float _dx, _dy;
	Direction _facing;

	bool _lookingUp;
	bool _lookingDown;
	bool _grounded;
	bool _allowJumping;
	bool _onSlope;
	bool _isCollidingBot;
	bool _isInvincible;

	int _maxHealth;
	int _currentHealth;

	// Items
	Weapon *_testWeapon;
	std::map<std::string, std::shared_ptr<Weapon>> _arsenal;
	std::string _currentWeaponHeld;


};

