#include <SDL.h>
#include "Level.h"
#include "Graphics.h"
#include "globals.h"
#include "Utils.h"
#include "Enemy.h"
#include "Healthpack.h"
#include "Pistol.h"
#include "PistolBullet.h"
#include "Rocketlauncher.h"


#include "tinyxml2.h"

#include <sstream>
#include <algorithm>
#include <cmath>
#include <iostream>
#include <memory>

using namespace tinyxml2;

Level::Level()
{
}

Level::Level(std::string mapName, Graphics& graphics) :
_mapName(mapName),
_size(Vector2::zero())
{
	this->loadMap(mapName, graphics);
}

void Level::update(int elapsedTime, Player& player)
{
	for (int i = 0; i < this->_animatedTileList.size(); i++) {
		this->_animatedTileList.at(i).update(elapsedTime);
	}

	for (int i = 0; i < this->_enemies.size(); i++) {
		this->_enemies.at(i)->update(elapsedTime, player);
	}
}

void Level::draw(Graphics& graphics)
{
	for (int i = 0; i < this->_tileList.size(); i++) {
		this->_tileList.at(i).draw(graphics);
	}

	for (int i = 0; i < this->_animatedTileList.size(); i++) {
		this->_animatedTileList.at(i).draw(graphics);
	}

	for (int i = 0; i < this->_enemies.size(); i++) {
		this->_enemies.at(i)->draw(graphics);
	}

}


std::vector<Rectangle> Level::checkTileCollisions(const Rectangle& other)
{
	std::vector<Rectangle> others;
	for (int i = 0; i < this->_collisionRects.size(); i++) {
		if (this->_collisionRects.at(i).collidesWith(other)) {
			others.push_back(this->_collisionRects.at(i));
		}
	}
	//if (others.size() > 0) 
	//std::cout << "Number of collisionRects colliding with player bounding: " << others.size() << std::endl;
	return others;
}

std::vector<Slope> Level::checkSlopeCollisions(const Rectangle& other)
{
	std::vector<Slope> others;
	for (int i = 0; i < _slopes.size(); i++) {
		if (this->_slopes.at(i).collidesWith(other)) {
			others.push_back(this->_slopes.at(i));
		}
	}
	return others;
}

std::vector<Door> Level::checkDoorCollisions(const Rectangle& other)
{
	std::vector<Door> others;
	for (int i = 0; i < _doorList.size(); i++) {
		if (this->_doorList[i].collidesWith(other)) {
			others.push_back(this->_doorList[i]);
		}
	}
	return others;
}

std::vector<std::shared_ptr<Enemy>> Level::checkEnemyCollision(const Rectangle& other)
{
	std::vector<std::shared_ptr<Enemy>> others;
	for (int i = 0; i < _enemies.size(); i++) {
		if (this->_enemies[i]->getBoundingBox().collidesWith(other) && this->_enemies[i]->getBoundingBox().isCollisionRect() == true) {
			others.push_back(this->_enemies[i]);
			printf("collision\n");
		}
	}
	return others;
}

std::vector<std::shared_ptr<Item>> Level::checkItemCollision(const Rectangle& other)
{
	std::vector<std::shared_ptr<Item>> others;
	for (int i = 0; i < _items.size(); i++) {
		if (this->_items[i]->getCollRect().collidesWith(other)) {
			others.push_back(this->_items[i]);

			for (int j = 0; j < this->_animatedTileList.size(); j++) {		// remove the AnimTile from the list so it isn't drawn anymore
				if (this->_animatedTileList[j].getID() == this->_items[i]->getId()) {
					this->_animatedTileList.erase(this->_animatedTileList.begin() + j);
					std::cout << "picked up: " << this->_items[i]->getItemDescription() << std::endl;
				}
			}
			this->_items.erase(this->_items.begin() + i);	// stop collision, remove Item from maps item vector

		}
	}
	return others;

}




void Level::loadMap(std::string mapName, Graphics& graphics)
{
	// Parse .tmx (xml) Tiles -> Vector
	XMLDocument doc;
	std::stringstream ss;


	ss << "content/maps/" << mapName << ".tmx";
	doc.LoadFile(ss.str().c_str());

	XMLElement *mapNode = doc.FirstChildElement("map");

	// Get width and the height of the whole map and store it in _size
	int width, height;
	mapNode->QueryIntAttribute("width", &width);	// No of tiles across
	mapNode->QueryIntAttribute("height", &height);	// No of tiles bot to top
	this->_size = Vector2(width, height);

	// Get the width and the height of the tiles
	int tileWidth, tileHeight;
	mapNode->QueryIntAttribute("tilewidth", &tileWidth);
	mapNode->QueryIntAttribute("tileheight", &tileHeight);
	this->_tileSize = Vector2(tileWidth, tileHeight);

	// Loading the tilesets of this level into tileset vector
	XMLElement *pTileset = mapNode->FirstChildElement("tileset");
	if (pTileset != NULL) {	// Linked List Nodes
		while (pTileset) {
			int firstgid;

			const char *source = pTileset->FirstChildElement("image")->Attribute("source");

			// form the correct link to the content folder
			std::string src(source);
			src.erase(0, 2);
			src.insert(0, "content");

			pTileset->QueryIntAttribute("firstgid", &firstgid);
			SDL_Texture *tex = SDL_CreateTextureFromSurface(graphics.getRenderer(), graphics.loadImage(src));
			this->_tilesets.push_back(Tileset(tex, firstgid));

			// Parse out animation for that tileset
			XMLElement *ptileA = pTileset->FirstChildElement("tile");
			if (ptileA != NULL) {
				while (ptileA) {
					AnimatedTileInfo ati;
					ati.StartTileId = ptileA->IntAttribute("id") + firstgid;
					ati.TilesetsFirstGid = firstgid;

					XMLElement *pAnimation = ptileA->FirstChildElement("animation");
					if (pAnimation != NULL) {
						while (pAnimation) {
							XMLElement *pFrame = pAnimation->FirstChildElement("frame");
							if (pFrame != NULL) {
								while (pFrame) {
									ati.TileIds.push_back(pFrame->IntAttribute("tileid") + firstgid);
									ati.Duration = pFrame->IntAttribute("duration");
									pFrame = pFrame->NextSiblingElement("frame");
								}
							}
							pAnimation = pAnimation->NextSiblingElement("animation");
						}
					}

					this->_animatedTileInfo.push_back(ati);

					// DEBUG
					/*std::cout << "Anim in Tileset with FirstGid = " << ati.TilesetsFirstGid << ", containing GIDs: ";
					for (auto i : ati.TileIds)
						std::cout << i << ", ";
					std::cout << std::endl;*/

					ptileA = ptileA->NextSiblingElement("tile");
				}
			}
			pTileset = pTileset->NextSiblingElement("tileset");	// Linked List Next Element
		}
	}



	//Loading the layers
	XMLElement *pLayer = mapNode->FirstChildElement("layer");
	if (pLayer != NULL) {
		while (pLayer) {
			// Loading the data element
			XMLElement *pData = pLayer->FirstChildElement("data");
			if (pData != NULL) {
				while (pData) {
					// Loading the tile element
					XMLElement *pTile = pData->FirstChildElement("tile");
					if (pTile != NULL) {
						int tileCounter = 0;
						AnimatedTileInfo ati;
						Tileset tls;
						int xx, yy;
						Vector2 finalTilePosition;
						Vector2 finalTilesetPosition;
						bool isAnimatedTile;


						while (pTile) {
							// Build each individual tile here
							// IF Gid = 0, no tile to be drawn
							if (pTile->IntAttribute("gid") == 0) {					// GID = 0, no tile to be drawn
								tileCounter++;
								if (pTile->NextSiblingElement("tile")) {			// if there are more tiles 
									pTile = pTile->NextSiblingElement("tile");		// Go on Node further and continue
									continue;
								}
								else {
									break;
								}
							}

							// Get the tileset for this specific gid
							int gid = pTile->IntAttribute("gid");

							for (int i = 0; i < this->_tilesets.size(); i++) {
								if (this->_tilesets[i].FirstGid <= gid) {
									//This is the tileset we want
									// Ex. Tileset1 {FirstGid = 1 ... 50 }, Tileset2 {FirstGid = 51 ... 100 }
									//If the parsed gid is bigger or equals than Tileset1.FirstGid or Tileset2.FirstGid, then it belongs to either Tileset1 or Tileset2
									tls = this->_tilesets.at(i);		// [] returns a reference, not copy
									//break;
								}
							}

							if (tls.FirstGid == -1) {
								//No tileset was found for this gid
								tileCounter++;
								if (pTile->NextSiblingElement("tile")) {
									pTile = pTile->NextSiblingElement("tile");
									continue;
								}
								else {
									break;
								}
							}

							// Get the position of the tile in the level
							xx = (tileCounter % width) * tileWidth;		// destX
							yy = (tileCounter / width) * tileHeight;	// destY
							finalTilePosition = Vector2(xx, yy);	// (x, y) in destImage

							//Calculate the position of the tile in the tileset
							finalTilesetPosition = this->getTilesetPosition(tls, gid, tileWidth, tileHeight);
							isAnimatedTile = false;
							

							for (int i = 0; i < _animatedTileInfo.size(); i++) {
								if (this->_animatedTileInfo.at(i).StartTileId == gid) {
									ati = this->_animatedTileInfo.at(i);					// Get the animatedTileInfo struct when an GID has been found
									ati._ID = tileCounter;
									isAnimatedTile = true;
									break;
								}
							}

							if (isAnimatedTile) {
								std::vector<Vector2> tilesetPositions;
								for (int i = 0; i < ati.TileIds.size(); i++) {
									// If a GID is found that matches the GID of the first frame of an entire animation
									// AnimatedTile is not ONE tile but contains a LIST of tilePositions
									// First get the correct AnimatedTileInfo that stores the GIDs of the frames that are part of the animation
									// Then loop through the LIST of needed GIDs and create Vector2s of their position on the tileset
									// Create an AnimatedTile object that holds that list 
									// finalTilePosition contains coordinates for the sourceRect 

									tilesetPositions.push_back(this->getTilesetPosition(tls, ati.TileIds.at(i), tileWidth, tileHeight));
								}

								AnimatedTile tile(tilesetPositions, ati.Duration, tls.Texture, Vector2(tileWidth, tileHeight), finalTilePosition, tileCounter);
								this->_animatedTileList.push_back(tile);


							}
							else {
								// Build the actual tile and add it to the level tile list
								// Check if tile is an AnimatedTile

								Tile tile(tls.Texture, Vector2(tileWidth, tileHeight),
									finalTilesetPosition, finalTilePosition);
								this->_tileList.push_back(tile);
							}
							tileCounter++;
							pTile = pTile->NextSiblingElement("tile");
						}
					}

					pData = pData->NextSiblingElement("data");
				}
			}


			pLayer = pLayer->NextSiblingElement("layer");
		}
	}

	//Parse out the collision
	XMLElement *pObjectGroup = mapNode->FirstChildElement("objectgroup");
	if (pObjectGroup != NULL) {
		while (pObjectGroup) {
			const char *name = pObjectGroup->Attribute("name");
			std::string str(name);
			if (str == "collisions") {
				XMLElement *pObject = pObjectGroup->FirstChildElement("object");
				if (pObject != NULL) {
					while (pObject) {
						float x, y, width, height;
						x = pObject->FloatAttribute("x");
						y = pObject->FloatAttribute("y");
						width = pObject->FloatAttribute("width");
						height = pObject->FloatAttribute("height");

						this->_collisionRects.push_back(Rectangle(
							std::ceil(x) * globals::SPRITE_SCALE,		// !!
							std::ceil(y) * globals::SPRITE_SCALE,
							std::ceil(width) * globals::SPRITE_SCALE,	// round up collisionRects for safety reasons (XML float coordinates -> SDL_Rect int coordinates)
							std::ceil(height) * globals::SPRITE_SCALE
							));
						//std::cout << _collisionRects.size() << std::endl;

						pObject = pObject->NextSiblingElement("object");
					}
				}

			}
			else if (str == "slopes") {
				XMLElement *pObject = pObjectGroup->FirstChildElement("object");
				if (pObject != NULL) {
					while (pObject) {
						std::vector<Vector2> points;
						Vector2 p1;

						p1 = Vector2(
							std::ceil(pObject->FloatAttribute("x")),
							std::ceil(pObject->FloatAttribute("y"))
							);

						XMLElement *pPolyline = pObject->FirstChildElement("polyline");
						if (pPolyline != NULL) {	// No while loop since only one polyline here
							std::vector<std::string> pairs;
							const char *pointString = pPolyline->Attribute("points");

							std::stringstream ss;
							ss << pointString;

							Utils::split(ss.str(), pairs, ' ');										// pairs("0,0", "12.4,15.3") usually, size = 2
							//std::cout << "pairs size: " << pairs.size() << std::endl;

							// Now we have each of the pairs: pair1: 0,0 , pair2: 20.375,-18.75
							// Loop through the pairs and split them into Vector2s and store them in points vector
							for (int i = 0; i < pairs.size(); i++) {
								std::vector<std::string> ps;
								Utils::split(pairs.at(i), ps, ',');
								points.push_back(Vector2(std::stoi(ps.at(0)), std::stoi(ps.at(1))));
							}

							//std::cout << "points size: " << points.size() << std::endl;
							int dx, dy;
							Vector2 temp = Vector2::zero();

							for (int i = 1; i < points.size(); i++) {				// points(Vec1(0,0), Vec2(12.4, 15.3)) usually, size = 2
								dx = points[i].x;
								dy = points[i].y;

								this->_slopes.push_back(Slope(
									Vector2((i == 1 ? p1.x : temp.x) * globals::SPRITE_SCALE, (i == 1 ? p1.y : temp.y) * globals::SPRITE_SCALE),
									Vector2((p1.x + dx) * globals::SPRITE_SCALE, (p1.y + dy) * globals::SPRITE_SCALE
									)));

								// DEBUG
								/*std::cout << "Slope " << i << "P1(" << p1.x * globals::SPRITE_SCALE << "," << p1.y * globals::SPRITE_SCALE
									<< "), P2(" << (p1.x + dx) * globals::SPRITE_SCALE << "," << (p1.y + dy) * globals::SPRITE_SCALE << ")" << std::endl;*/

								temp.x = p1.x + dx;
								temp.y = p1.y + dy;

							}
							//std::cout << "slopes size: " << this->_slopes.size() << std::endl;
						}

						pObject = pObject->NextSiblingElement("object");
					}
				}
			}
			else if (str == "spawn points") {
				XMLElement *pObject = pObjectGroup->FirstChildElement("object");
				if (pObject != NULL) {
					while (pObject) {
						float x = pObject->FloatAttribute("x");
						float y = pObject->FloatAttribute("y");
						const char *name = pObject->Attribute("name");

						std::string str(name);
						if (str == "Player") {
							this->_spawnPoint = Vector2(std::ceil(x) * globals::SPRITE_SCALE,
								std::ceil(y) * globals::SPRITE_SCALE);
						}

						pObject = pObject->NextSiblingElement("object");
					}
				}
			}
			else if (str == "doors") {
				XMLElement *pObject = pObjectGroup->FirstChildElement("object");
				while (pObject) {
					float x = pObject->FloatAttribute("x");
					float y = pObject->FloatAttribute("y");
					float width = pObject->FloatAttribute("width");
					float height = pObject->FloatAttribute("height");

					Rectangle rect(x, y, width, height);

					XMLElement *pProperties = pObject->FirstChildElement("properties");
					if (pProperties != NULL) {
						XMLElement *pProperty = pProperties->FirstChildElement("property");
						if (pProperty != NULL) {
							const char *name = pProperty->Attribute("name");
							std::string str(name);
							if (str == "destination") {
								const char *value = pProperty->Attribute("value");
								std::string str(value);
								Door door(rect, str);

								this->_doorList.push_back(door);
							}
							pProperty = pProperty->NextSiblingElement("property");
						}
						pProperties = pProperties->NextSiblingElement("properties");
					}

					pObject = pObject->NextSiblingElement("object");
				}
			}
			else if (str == "enemies") {
				float x, y;
				XMLElement *pObject = pObjectGroup->FirstChildElement("object");

				if (pObject != NULL) {
					while (pObject) {
						x = pObject->FloatAttribute("x");
						y = pObject->FloatAttribute("y");

						const char *name = pObject->Attribute("name");
						std::string str(name);

						if (str == "bat") {
							this->_enemies.push_back(std::make_shared<Bat>(
								graphics,
								Vector2(
								std::floor(x) * globals::SPRITE_SCALE,
								std::floor(y) * globals::SPRITE_SCALE)));
						}

						if (str == "trump") {
							this->_enemies.push_back(std::make_shared<Trump>(
								graphics,
								Vector2(
								std::floor(x) * globals::SPRITE_SCALE,
								std::floor(y) * globals::SPRITE_SCALE)));
						
						}

						pObject = pObject->NextSiblingElement("object");
					}
				}
			}
			else if (str == "items") {
				XMLElement *pObject = pObjectGroup->FirstChildElement("object");
				if (pObject != NULL) {
					while (pObject) {
						std::shared_ptr<Item> item;
		
						float x = pObject->FloatAttribute("x");
						float y = pObject->FloatAttribute("y");
						float height = pObject->FloatAttribute("height");
						float width = pObject->FloatAttribute("width");
						int id = 0;

						Rectangle rect(
							std::ceil(x) * globals::SPRITE_SCALE,
							std::ceil(y) * globals::SPRITE_SCALE,
							std::ceil(width) * globals::SPRITE_SCALE,
							std::ceilf(height) * globals::SPRITE_SCALE
						);

						XMLElement *pProperties = pObject->FirstChildElement("properties");
						if (pProperties != NULL) {
							while (pProperties) {
								XMLElement *pProperty = pProperties->FirstChildElement("property");
								if (pProperty != NULL) {
									while (pProperty) {
										const char *name = pProperty->Attribute("name");
										std::string str(name);
										if (str == "id") {												//  every Item is an AnimSprite in the map and is identified by the tileCounter
											id = pProperty->IntAttribute("value");						//  (exact position on the map)
										}
										pProperty = pProperty->NextSiblingElement("property");
									}
								}
								pProperties = pProperties->NextSiblingElement("properties");
							}

						}

						const char *name = pObject->Attribute("name");
						std::string itemname(name);

						if (itemname == "pistol") {
							// Add the weapon to the level's item vector
							item = std::make_shared<Pistol>(id, rect, graphics);

							// Add the default ammunition to the weapon
							std::dynamic_pointer_cast<Weapon>(item)->addAmmunition(new PistolBullet(graphics));
			
							this->_items.push_back(item);
							printf("Pistol added\n");
						}
						else if (itemname == "healthpack") {
							item = std::make_shared<Healthpack>(id, ItemDescriptions["healthpack"], rect);
							this->_items.push_back(item);
							printf("Healthpack added\n");
						}
						else if (itemname == "save") {

						}
						else if (itemname == "rifle") {
						/*	item = std::make_shared<Rifle>(id, ItemDescriptions["rifle"], rect, WeaponProperty(),
								graphics, "content/sprites/Arms.png", 0, 0, 24, 16, 0, 0, 100);
							this->_items.push_back(item);*/
						}
						else if (itemname == "rocketlauncher") {
							item = std::make_shared<Rocketlauncher>(id, rect, graphics);
							this->_items.push_back(item);
							printf("Rocketlauncher added\n");
						}
						else {
							printf("Unknown item found, check XML objectgroup items\n");
						}
						pObject = pObject->NextSiblingElement("object");
					}

				}
			}
			pObjectGroup = pObjectGroup->NextSiblingElement("objectgroup");
		}
	}

}

const Vector2 Level::getPlayerSpawnPoint() const
{
	return this->_spawnPoint;
}

Vector2 Level::getTilesetPosition(Tileset& tls, int gid, int tileWidth, int tileHeight)
{
	int tilesetWidth, tilesetHeight;
	SDL_QueryTexture(tls.Texture, NULL, NULL, &tilesetWidth, &tilesetHeight);	// also via XML query 

	int tsxx = ((gid - tls.FirstGid) % (tilesetWidth / tileWidth)) * tileWidth;
	int tsyy = ((gid - tls.FirstGid) / (tilesetWidth / tileWidth)) * tileHeight;
	Vector2 finalTilesetPosition = Vector2(tsxx, tsyy);	// (x, y) in tilesetImage

	return finalTilesetPosition;
}


