#pragma once

#include "globals.h"
#include "Tile.h"
#include "Rectangle.h"
#include "Slope.h"
#include "AnimatedTile.h"
#include "Door.h"
#include "Enemy.h"
#include "Item.h"

#include <string>
#include <vector>
#include <memory>

class Graphics;
struct SDL_Texture;
struct SDL_Rect;
struct Tileset;

class Level
{

public:
	Level();
	Level(std::string mapName, Graphics& graphics);

	void update(int elapsedTime, Player& player);
	void draw(Graphics& graphics);

	std::vector<Rectangle> checkTileCollisions(const Rectangle& other);
	std::vector<Slope> checkSlopeCollisions(const Rectangle& other);
	std::vector<Door> checkDoorCollisions(const Rectangle& other);
	std::vector<std::shared_ptr<Enemy>> checkEnemyCollision(const Rectangle& other);
	std::vector<std::shared_ptr<Item>> checkItemCollision(const Rectangle& other);

	const Vector2 getPlayerSpawnPoint() const;

	const std::vector<Slope>& getSlopes() const { return this->_slopes; };
	const std::vector<Door>& getDoors() const { return this->_doorList; };
	const std::vector<std::shared_ptr<Enemy>>& getEnemies() const { return this->_enemies; };	// !! Polymorphism only works with References and Pointers, hence 
	const std::vector<std::shared_ptr<Item>>& getItems() const { return this->_items; };	// we create an array of pointers (shared_ptrs) instead of objects!!!
	
	
	Level& operator= (const Level& other) = default;	// assignment operator is needed but default is OK
	~Level();

private:
	std::string _mapName;
	Vector2 _spawnPoint;
	
	Vector2 _size;			// Size of the map (number tiles width, numbers tiles height)
	Vector2 _tileSize;

	SDL_Texture *_backgroundTexture;

	std::vector<Tile> _tileList;
	std::vector<Tileset> _tilesets;
	std::vector<Rectangle> _collisionRects;
	std::vector<Slope> _slopes;

	std::vector<AnimatedTile> _animatedTileList;
	std::vector<AnimatedTileInfo> _animatedTileInfo;

	std::vector<Door> _doorList;

	std::vector<std::shared_ptr<Item>> _items;
	std::vector<std::shared_ptr<Enemy>> _enemies;

	/* void loadMap
	 * Loads a map (only inside Level class)
	 */
	void loadMap(std::string mapName, Graphics& graphics);

	Vector2 getTilesetPosition(Tileset& tls, int gid, int tileWidth, int tileHeight);

};

// Tileset structure 
struct Tileset {
	SDL_Texture *Texture;
	int FirstGid;

	Tileset() {
		this->FirstGid = -1;
	}

	Tileset(SDL_Texture *texture, int FirstGid) {
		this->Texture = texture;
		this->FirstGid = FirstGid;
	}
};

