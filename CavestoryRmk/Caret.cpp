#include "Caret.h"
#include "Graphics.h"

PistolCaret::PistolCaret(Graphics& graphics) :
Caret(graphics, "content/sprites/Caret.png", 0, 0, 17, 10, 0, 0, 100)
{
	this->setupAnimation();
	this->setVisible(false);
}


void PistolCaret::update(int elapsedTime)
{
	if (this->_visible == true) {								// Animation ONLY if weapon is shot, this way update() and draw() are not called when player is not shooting!
		Sprite::update();
		this->_timeElapsed += elapsedTime;

		if (this->_timeElapsed >= this->_timeToUpdate) {
			this->_timeElapsed -= this->_timeToUpdate;
			if (this->_frameIndex < this->_animations[this->_currentAnimation].size() - 1) {
				this->_frameIndex++;
			}
			else {
				if (this->_currentAnimationOnce == true) {
					this->setVisible(false);
				}
				this->stopAnimation();
			}
		}
	}

}

void PistolCaret::draw(Graphics& graphics, int x, int y)
{
	if (this->_visible == true) {

		SDL_Rect destinationRectangle = { 0, 0, 0, 0 };

		destinationRectangle.w = this->_sourceRect.w * globals::SPRITE_SCALE;
		destinationRectangle.h = this->_sourceRect.h * globals::SPRITE_SCALE;

		// additional offset between player and wall is needed
		// X = players bounding box X
		// WallOffset is distance between player bounding box and collisionRect
		// 13 is a smoothing factor, the animation is played not at the edge of the collRect, but in the middle
		if (this->_currentAnimation == "PistolWallCaretLeft") {
			destinationRectangle.x = x - std::abs(this->getWallOffset().x - x) - 13;
			//std::cout << std::abs(this->getWallOffset().x - x) << std::endl;
		}
		else if (this->_currentAnimation == "PistolWallCaretRight") {
			destinationRectangle.x = x + std::abs(this->getWallOffset().x - (x + player_constants::PLAYER_WIDTH)) + 13;
			//std::cout << std::abs(this->getWallOffset().x - x) << std::endl;
		}
		else {
			destinationRectangle.x = x + this->_offsets[this->_currentAnimation].x;
		}


		// SHOOT UP
		if (this->_currentAnimation == "PistolCaretUp") {
			destinationRectangle.y = y - std::abs(this->getWallOffset().y - y) - 10;

		}
		else {
			destinationRectangle.y = y + this->_offsets[this->_currentAnimation].y;
		}


		SDL_Rect sourceRect = this->_animations[this->_currentAnimation][this->_frameIndex];
		graphics.blitSurface(this->_spriteSheet, &sourceRect, &destinationRectangle);
	}
}


void PistolCaret::animationDone(std::string currentAnimation)
{
	this->setVisible(false);
}

void PistolCaret::setupAnimation()
{
	// at pistol muzzle left right
	this->addAnimation(4, 0, 48, "PistolCaretLeft", 16, 16, Vector2(-23, 10));
	this->addAnimation(4, 0, 48, "PistolCaretRight", 16, 16, Vector2(23, 10));

	// at pistol muzzle topleft topright
	this->addAnimation(4, 0, 48, "PistolCaretUpLeft", 16, 16, Vector2(2, -18));
	this->addAnimation(4, 0, 48, "PistolCaretUpRight", 16, 16, Vector2(-2, -18));

	// wall left or right side
	this->addAnimation(4, 11, 0, "PistolWallCaretLeft", 16, 16, Vector2(0, 10));
	this->addAnimation(4, 11, 0, "PistolWallCaretRight", 16, 16, Vector2(0, 10));

	// wall top side
	this->addAnimation(4, 11, 0, "PistolCaretUp", 16, 16, Vector2(0, 0));
}