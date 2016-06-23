#include "Ammunition.h"

#include "Graphics.h"

Ammunition::Ammunition()
{
}

Ammunition::Ammunition(Rectangle hitbox, Vector2 offset, Graphics& graphics, const std::string& filePath, int sourceX, int sourceY, int width, int height,
	float posX, float posY, float timeToUpdate) :
	AnimatedSprite(graphics, filePath, sourceX, sourceY, width, height, posX, posY, timeToUpdate),
	_hitbox(hitbox),
	_direction(RIGHT),
	_offset(offset),
	_dx(0.0f),
	_dy(0.0f)
{

}

void Ammunition::resetBulletAnimation()
{
	this->setVisible(false);
	this->_dx = 0;
	this->_dy = 0;

}





Bullet::Bullet(Rectangle hitbox, Vector2 offset, Graphics& graphics, const std::string& filePath, int sourceX, int sourceY, int width, int height,
	float posX, float posY, float timeToUpdate) :
	Ammunition(hitbox, offset, graphics, filePath, sourceX, sourceY, width, height, posX,
	posY, timeToUpdate)
{}



