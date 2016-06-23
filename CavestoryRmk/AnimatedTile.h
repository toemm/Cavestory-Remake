#pragma once

#include "Tile.h"
#include "globals.h"

#include <vector>

/* class AnimatedTile
 * Since it contains not only one position but a List of tiles it should be called AnimatedTiles
 */


class AnimatedTile :
	public Tile
{
public:
	AnimatedTile(std::vector<Vector2> tilesetPositions, int duration, SDL_Texture *tileset, Vector2 size, Vector2 position, int ID);

	void update(int elapsedTime);
	void draw(Graphics& graphics);

	const int inline getID() const { return this->_ID; }
	void inline setAnimate(bool flag) { this->_animate = flag; }

	~AnimatedTile();


protected:
	int _amountOfTime = 0;
	bool _notDone = false;


private:
	int _ID;
	std::vector<Vector2> _tilesetPositions;
	int _tileToDraw;
	int _duration;
	bool _animate;
	


};

struct AnimatedTileInfo {
	int _ID;						// Tile ID to identify a collRect with the correct AnimTile
	int TilesetsFirstGid;			// Gid of the Tileset
	int StartTileId;				// usually TileIds[0]
	std::vector<int> TileIds;		// Stores the Gids of the tiles of the specific tileset that take part in the animation
	int Duration;
};
	