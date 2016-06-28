#include <algorithm>

#include <SDL.h>
#include <SDL_ttf.h>
#include "Game.h"
#include "Graphics.h"
#include "Input.h"
#include "Door.h"
#include "Rectangle.h"
#include "Slope.h"
#include "Item.h"
#include "Camera.h"
#include "Text.h"

#define LIMIT_FPS
#undef LIMIT_FPS
#define DEBUG
//#undef DEBUG

namespace FPS {
	const int FPS = 60;
	const int MAX_FRAME_TIME = 1000 / FPS;
}


Game::Game()
{
	SDL_Init(SDL_INIT_EVERYTHING);
	this->gameLoop();
}


Game::~Game()
{
}

void Game::gameLoop()
{
	Graphics graphics;

	this->_graphics = graphics;						// assignment-op copies important pointers
	this->_level = Level("bigmap1", _graphics);		// assignment-op, creates copy, careful with pointers
	this->_player = Player(_graphics, _level.getPlayerSpawnPoint());
	this->_hud = Hud(_graphics);

	this->initText();

	// Play music from the start
	// Audio::playMusic("maintheme", -1);

	auto LAST_UPDATE_TIME = SDL_GetTicks();
	this->_running = true;

	// Start the game loop
	while (this->_running == true) {
		this->input();

		const int CURRENT_TIME_MS = SDL_GetTicks();
		int ELAPSED_TIME_MS = CURRENT_TIME_MS - LAST_UPDATE_TIME;

#ifdef LIMIT_FPS
		if (ELAPSED_TIME_MS < FPS::MAX_FRAME_TIME) {
			update(FPS::MAX_FRAME_TIME);
			SDL_Delay(FPS::MAX_FRAME_TIME - ELAPSED_TIME_MS);
		}
#else
		this->update(std::min(ELAPSED_TIME_MS, FPS::MAX_FRAME_TIME));
#endif
		LAST_UPDATE_TIME = CURRENT_TIME_MS;

		this->draw();

	}


}

void Game::input()
{
	this->_input.beginNewFrame();								// reset keys pressed
	if (SDL_PollEvent(&this->_sdlevent)) {
		switch (this->_sdlevent.type) {
		case SDL_KEYDOWN:
			if (this->_sdlevent.key.repeat == 0) {
				this->_input.keyDownEvent(this->_sdlevent);
			}
			//std::cout << "Pressed: " << SDL_GetKeyName(this->_sdlevent.key.keysym.sym) << std::endl;
			break;

		case SDL_KEYUP:
			this->_input.keyUpEvent(this->_sdlevent);
			//std::cout << "Released: " << SDL_GetKeyName(this->_sdlevent.key.keysym.sym) << std::endl;
			break;

		default:
			break;
		}
	}

	// Selective input if player is dead (restart (Y/N))
	if (this->_player.isPlayerDead() == true) {
		if (this->_input.isKeyHeld(SDL_SCANCODE_Z))
			this->restart();

		if (this->_input.isKeyHeld(SDL_SCANCODE_N))
			this->_running = false;
	}
	else {
		// System
		if (this->_input.wasKeyPressed(SDL_SCANCODE_ESCAPE))
			this->_running = false;
		if (this->_input.wasKeyPressed(SDL_SCANCODE_O))
			this->restart();

		// Player animation
		if (this->_input.isKeyHeld(SDL_SCANCODE_A))
			this->_player.moveLeft();

		if (this->_input.isKeyHeld(SDL_SCANCODE_D))
			this->_player.moveRight();

		if (this->_input.wasKeyPressed(SDL_SCANCODE_SPACE) == true)
			this->_player.jump();

		if (this->_input.isKeyHeld(SDL_SCANCODE_W))
			this->_player.lookUp();

		if (this->_input.isKeyHeld(SDL_SCANCODE_S) == true)
			this->_player.lookDown();

		if (this->_input.wasKeyReleased(SDL_SCANCODE_W) == true)
			this->_player.stopLookingUp();

		if (this->_input.wasKeyReleased(SDL_SCANCODE_S) == true)
			this->_player.stopLookingDown();

		if (!this->_input.isKeyHeld(SDL_SCANCODE_A) && !this->_input.isKeyHeld(SDL_SCANCODE_D))
			this->_player.stopMoving();

		// Items (Weapons)
		if (this->_input.wasKeyPressed(SDL_SCANCODE_1) == true)		// Key 1 = No Weapon
			this->_player.changeWeapon("none");

		if (this->_input.wasKeyPressed(SDL_SCANCODE_2) == true)		// Key 2 = Pistol
			this->_player.changeWeapon("pistol");

		if (this->_input.wasKeyPressed(SDL_SCANCODE_3) == true)		// Key 3 = Rifle
			this->_player.changeWeapon("rifle");

		if (this->_input.wasKeyPressed(SDL_SCANCODE_4) == true)		// Key 4 = Rocketlauncher
			this->_player.changeWeapon("rocketlauncher");

		if (this->_input.wasKeyPressed(SDL_SCANCODE_X) == true)	{	// Key x = firing
			this->_player.shootWeapon();
			Audio::playSound(this->_player.getCurrentWeapon()->getItemDescription(), 0);
		}

		// Menu
		if (this->_input.wasKeyPressed(SDL_SCANCODE_P) == true)	{	// Key P = Pause music/Resume music
			if (Audio::isPaused == true) {
				Audio::playMusic("maintheme", -1);
			}
			else {
				Audio::pauseMusic();
			}
		}

	}


}

void Game::draw()
{
	this->_graphics.clear();

	this->_level.draw(this->_graphics);
	this->_player.draw(this->_graphics);
	this->_hud.draw(this->_graphics);

	if (this->_player.isPlayerDead() == true) {
		this->_gameTexts["DeathRestart"].draw(this->_graphics);
	}




#ifdef DEBUG
	this->drawDebugLines();
#endif


	this->_graphics.flip();
}

void Game::update(int elapsedTime)
{
	// Check for player death
	if (this->_player.isPlayerDead() == true) {
		this->_player.stopMoving();						// dx/dy = 0
		this->_player.playAnimation("TombstoneIdle");	// tombstone as death animation
		this->_player.changeWeapon("none");				// so tombstone doesn't show weapon sprite at its side

		this->_gameTexts["DeathRestart"].update();		// For the future
	}


	//std::cout << elapsedTime << std::endl;
	this->_player.update(elapsedTime);			// update the frameIndex of the AnimSprite object
	this->_level.update(elapsedTime, this->_player);
	this->_hud.update(elapsedTime, this->_player);
	this->_camera.update(elapsedTime, this->_player);

	// Check slopes
	std::vector<Slope> otherSlopes = this->_level.checkSlopeCollisions(this->_player.getBoundingBox());
	if (otherSlopes.size() > 0) {
		this->_player.handleSlopeCollisions(otherSlopes);
	}

	// Check collisions
	std::vector<Rectangle> otherRects;
	otherRects = this->_level.checkTileCollisions(this->_player.getBoundingBox());
	if (otherRects.size() > 0) {
		// Player collided with atleast one tile. Handle it.
		this->_player.handleTileCollisions(otherRects);
	}

	// Check door collisions
	std::vector<Door> otherDoors;
	otherDoors = this->_level.checkDoorCollisions(this->_player.getBoundingBox());
	if (otherDoors.size() > 0) {
		this->_player.handleDoorCollision(otherDoors, this->_level, this->_graphics);
	}

	// Check enemy collision
	std::vector<std::shared_ptr<Enemy>> otherEnemies;
	if (this->_player.isInvincible() == false) {	// Only check for enemy collision if the player is not invincible
		otherEnemies = this->_level.checkEnemyCollision(this->_player.getBoundingBox());
		if (otherEnemies.size() > 0) {
			this->_player.handleEnemyCollisions(otherEnemies);
		}
	}




	// Check item collision
	std::vector<std::shared_ptr<Item>> otherItems;
	otherItems = this->_level.checkItemCollision(this->_player.getBoundingBox());
	if (otherItems.size() > 0) {
		this->_player.handleItemCollisions(otherItems);
	}

	// Check bullet collisions
	if (this->_player.getWeaponHeldString() != "none" && this->_player.getWeaponHeldString() != "rocketlauncher") {		// rocketlauncher not implemented
		if (this->_player.getCurrentWeapon()->getAmmunition()->getBoundingBox().isCollisionRect() == true) {			// safety measure, only check collision if the weapon is fired (otherwise rect is undef)

			// Tile Collision
			std::vector<Rectangle> otherBulletRectsTile;
			otherBulletRectsTile = this->_level.checkTileCollisions(this->_player.getCurrentWeapon()->getAmmunition()->getBoundingBox());
			if (otherBulletRectsTile.size() > 0) {
				this->_player.getCurrentWeapon()->getAmmunition()->handleTileCollisions(otherBulletRectsTile);
			}

			// Enemy collision
			std::vector<std::shared_ptr<Enemy>> hitEnemies;
			hitEnemies = this->_level.checkEnemyCollision(this->_player.getCurrentWeapon()->getAmmunition()->getBoundingBox());
			if (hitEnemies.size() > 0) {
				this->_player.getCurrentWeapon()->getAmmunition()->handleEnemyCollisions(hitEnemies);
			}

		}


	}

}

void Game::restart()
{
	// Restart the level
	// TODO: CLEANUP RESSOURCES CREATED IN THESE OBJECTS
	this->_level = Level("bigmap1", this->_graphics);
	this->_player = Player(this->_graphics, this->_level.getPlayerSpawnPoint());
}

void Game::initText()
{
	// Store text objects for the game object in a map for easier and cleaner access when the texts are needed
	this->_gameTexts.insert({
		"DeathRestart",
		Text(this->_graphics, "You died. Restart? (Y/N)", Vector2(-200, -100), 20, 0, 0, text::fontDir, text::green)
	});
}


























void Game::drawDebugLines()
{


	// DEBUG: Draw slope lines
	std::vector<Slope> slopes = this->_level.getSlopes();
	for (int i = 0; i < slopes.size(); i++) {

		SDL_RenderDrawLine(
			this->_graphics.getRenderer(),
			slopes[i].getP1().x - Camera::cameraRect.x,
			slopes[i].getP1().y - Camera::cameraRect.y,
			slopes[i].getP2().x - Camera::cameraRect.x,
			slopes[i].getP2().y - Camera::cameraRect.y);
	}

	// DEBUG: Draw door rectangles
	std::vector<Door> doors = this->_level.getDoors();
	for (int i = 0; i < doors.size(); i++) {

		SDL_Rect rect = {
			doors[i].getRectangle().getLeft() - Camera::cameraRect.x,
			doors[i].getRectangle().getTop() - Camera::cameraRect.y,
			doors[i].getRectangle().getWidth(),
			doors[i].getRectangle().getHeight()
		};

		SDL_RenderDrawRect(this->_graphics.getRenderer(), &rect);

	}

	// DEBUG: Draw enemy bounding box
	std::vector<std::shared_ptr<Enemy>> enemies = this->_level.getEnemies();
	for (int i = 0; i < enemies.size(); i++) {

		SDL_Rect rect = {
			enemies[i]->getBoundingBox().getLeft() - Camera::cameraRect.x,
			enemies[i]->getBoundingBox().getTop() - Camera::cameraRect.y,
			enemies[i]->getBoundingBox().getWidth(),
			enemies[i]->getBoundingBox().getHeight()
		};

		int d = SDL_RenderDrawRect(this->_graphics.getRenderer(), &rect);
	}

	// Items bounding boxes
	std::vector<std::shared_ptr<Item>> items = this->_level.getItems();
	for (int i = 0; i < items.size(); i++) {

		SDL_Rect rect = {
			items[i]->getCollRect().getLeft() - Camera::cameraRect.x,
			items[i]->getCollRect().getTop() - Camera::cameraRect.y,
			items[i]->getCollRect().getWidth(),
			items[i]->getCollRect().getHeight()
		};

		SDL_RenderDrawRect(this->_graphics.getRenderer(), &rect);
	}

	// Draw weapon hitb
	if (this->_player.getWeaponHeldString() != "none") {

		Rectangle rect = this->_player.getCurrentWeapon()->getAmmunition()->getBoundingBox();

		SDL_Rect sdlrect = {
			rect.getLeft() - Camera::cameraRect.x,
			rect.getTop() - Camera::cameraRect.y,
			rect.getWidth(),
			rect.getHeight()
		};

		SDL_RenderDrawRect(this->_graphics.getRenderer(), &sdlrect);
		SDL_RenderFillRect(this->_graphics.getRenderer(), &sdlrect);
	}
}





















