#pragma once

#include "AnimatedSprite.h"
#include "globals.h"
#include "Rectangle.h"
#include "Caret.h"
#include "Enemy.h"

#include <memory>

class Graphics;

class Ammunition :
	public AnimatedSprite
{
public:
	Ammunition();
	Ammunition(Rectangle hitbox, Vector2 offset, Graphics& graphics, const std::string& filePath, int sourceX, int sourceY, int width, int height,
		float posX, float posY, float timeToUpdate);

	virtual void handleTileCollisions(std::vector<Rectangle>& others) = 0;
	virtual void handleEnemyCollisions(std::vector<std::shared_ptr<Enemy>>& others) = 0;

	inline Direction getDirection() const { return this->_direction; }
	inline void setDirection(Direction dir) { this->_direction = dir; }
	inline const Vector2 getBulletOffset() const { return this->_offset; }

protected:
	Rectangle _hitbox;
	float _dx, _dy;
	void resetBulletAnimation();

private:
	Direction _direction;
	Vector2 _offset;
};





class Bullet :
	public Ammunition
{
public:
	Bullet(Rectangle hitbox, Vector2 offset, Graphics& graphics, const std::string& filePath, int sourceX, int sourceY, int width, int height,
		float posX, float posY, float timeToUpdate);


	inline Caret *getCaret() const { return this->_caret; }
	inline void setCaret(Caret *caret) { this->_caret = caret; }
	
private:
	Caret *_caret;

};







