#include "PistolBullet.h"
#include "Enemy.h"

#include <memory>


PistolBullet::PistolBullet(Graphics& graphics) :
Bullet(
Rectangle(0, 0, 3, 3),			// Bullet Hitbox
Vector2(5, 18),					// Offset Vector to place it over player hands, only works for left right, not when looking up...
graphics, "content/sprites/Bullet.png", 0, 0, 17, 10, 0, 0, 100)
{
	this->setCaret(new PistolCaret(graphics));
	this->setupAnimation();
	this->setVisible(false);
	this->_boundingBox.setCollision(false);
}

void PistolBullet::update(int elapsedTime)
{
	// True only, if player.fire() is called
	if (this->_visible == true) {
		this->_boundingBox.setCollision(true);	// safety measure, collision is only checked if weapon is fired, otherwise the bulletRect is undefined

		// If the bullet is shot sideways, only update x-coordinates and set collRect accordingly
		if (this->_currentAnimation == "BulletShootRight" || this->_currentAnimation == "BulletShootLeft") {
			if (std::fabs(this->_dx) <= bullet_physics::maxDistance) {
				this->_dx += this->getDirection() == RIGHT ? bullet_physics::xVelocity : -bullet_physics::xVelocity;
			}
			else {									  // max distance is reached
				this->resetBulletAnimation();		  // set _dx, _dy to zero and disable visibility, so the bullet isn't updated and drawn
			}

			this->_boundingBox = Rectangle(
				this->_x + (this->getDirection() == RIGHT ? 45 : -20),	// match hitbox with bullet		
				this->_y + this->getBulletOffset().y,
				this->_hitbox.getWidth() * globals::SPRITE_SCALE,
				this->_hitbox.getHeight() * globals::SPRITE_SCALE
				);

		}
		// If the bullet is shot above
		else if (this->_currentAnimation == "BulletShootUpRight" || this->_currentAnimation == "BulletShootUpLeft") {
			if (std::fabs(this->_dy) <= bullet_physics::maxDistance) {
				this->_dy += -bullet_physics::yVelocity;	// the bullets travels upwards with y coordinates getting lower
			}
			else {
				this->resetBulletAnimation(); // set _dx, _dy to zero and disable visibility, so the bullet isn't updated and drawn
			}
			this->_boundingBox = Rectangle(
				this->_x + (this->getDirection() == RIGHT ? 11 : 17),	// match hitbox with bullet		
				this->_y - 10,
				this->_hitbox.getWidth() * globals::SPRITE_SCALE,
				this->_hitbox.getHeight() * globals::SPRITE_SCALE
				);
		}




	}
	else {
		this->_boundingBox.setCollision(false);
	}

	this->getCaret()->update(elapsedTime);
}

void PistolBullet::draw(Graphics& graphics, int x, int y)
{
	// x, y position of the bullet's collRect
	this->_x = x + this->_dx;
	this->_y = y + this->_dy;

	//std::cout << "x: " << this->_x << ", y: " << this->_y << std::endl;

	if (this->_visible == true) {
		AnimatedSprite::draw(graphics, this->_x, this->_y);
	}


	this->getCaret()->draw(graphics, x, y);
}


void PistolBullet::animationDone(std::string currentAnimation)
{
	this->setVisible(false);
}

void PistolBullet::setupAnimation()
{
	// left & right
	this->addAnimation(1, 3, 20, "BulletShootRight", 21, 10, Vector2(25, 13));
	this->addAnimation(1, 5, 20, "BulletShootLeft", 19, 10, Vector2(-25, 13));

	// up left, up right
	this->addAnimation(1, 7, 16, "BulletShootUpRight", 16, 16, Vector2(-3, -2));
	this->addAnimation(1, 7, 16, "BulletShootUpLeft", 16, 16, Vector2(3, -2));

}


void PistolBullet::handleTileCollisions(std::vector<Rectangle>& others)
{
	// bullet's bounding box has collided with level rectangle
	this->resetBulletAnimation();

	// play wallhit caret
	// Calculate offset for caret animation
	// Only 1 collision possible, therefore take the only element in the array
	int CollisionWallX = 0;
	int CollisionWallY = 0;

	// shoot right, get left side, calculate dx
	if (this->_currentAnimation == "BulletShootRight") {
		CollisionWallX = others[0].getLeft();
		this->getCaret()->playAnimation("PistolWallCaretRight");
	} // shoot left, get right side, calculate dx
	else if (this->_currentAnimation == "BulletShootLeft") {
		CollisionWallX = others[0].getRight();
		this->getCaret()->playAnimation("PistolWallCaretLeft");
	} // shoot up, get bottom side, calculate dy
	else if (this->_currentAnimation == "BulletShootUpRight" || this->_currentAnimation == "BulletShootUpLeft") {
		CollisionWallY = others[0].getBottom();
		this->getCaret()->playAnimation("PistolCaretUp");
	}

	this->getCaret()->setVisible(true);
	this->getCaret()->setWallOffset(Vector2(CollisionWallX, CollisionWallY));

}

void PistolBullet::handleEnemyCollisions(std::vector<std::shared_ptr<Enemy>>& others)
{
	// If the bullet's BB hits an enemy
	this->resetBulletAnimation();
	others[0]->gainHealth(-1);



}
