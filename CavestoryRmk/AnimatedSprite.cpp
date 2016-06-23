#include "AnimatedSprite.h"
#include "Graphics.h"


AnimatedSprite::AnimatedSprite()
{
}

AnimatedSprite::AnimatedSprite(Graphics& graphics, const std::string& filePath, int sourceX, int sourceY, int width, int height,
	float posX, float posY, float timeToUpdate) :
	Sprite(graphics, filePath, sourceX, sourceY, width, height, posX, posY),
	_frameIndex(0),
	_timeToUpdate(timeToUpdate),
	_timeElapsed(0),
	_visible(true),
	_currentAnimationOnce(false),
	_currentAnimation("") {}

void AnimatedSprite::addAnimation(int frames, int x, int y, std::string name, int width, int height, Vector2 offset)
{
	std::vector<SDL_Rect> rectangles;
	for (int i = 0; i < frames; i++) {
		SDL_Rect newRect = { (i + x) * width, y, width, height };
		rectangles.push_back(newRect);
	}

	this->_animations.insert({ name, rectangles });
	this->_offsets.insert({ name, offset });


}

void AnimatedSprite::resetAnimations()
{
	this->_animations.clear();
	this->_offsets.clear();
}


void AnimatedSprite::playAnimation(std::string animation, bool once)
{
	this->_currentAnimationOnce = once;
	if (this->_currentAnimation != animation) {
		this->_currentAnimation = animation;
		this->_frameIndex = 0;
	}
}

void AnimatedSprite::setVisible(bool visible)
{
	this->_visible = visible;
}

void AnimatedSprite::stopAnimation()
{
	this->_frameIndex = 0;
	this->animationDone(this->_currentAnimation);
}

void AnimatedSprite::update(int elapsedTime)
{
	Sprite::update();
	this->_timeElapsed += elapsedTime;

	//std::cout << elapsedTime << std::endl;
	//if (this->_timeElapsed == _timeToUpdate)
	//	std::cout << "\a" << std::endl;

	if (this->_timeElapsed >= this->_timeToUpdate) {
		this->_timeElapsed -= this->_timeToUpdate;
		if (this->_frameIndex < this->_animations[this->_currentAnimation].size() - 1) {		// increment the frameIndex
			this->_frameIndex++;
		} else {
			if (this->_currentAnimationOnce == true) {
				this->setVisible(false);
			}
			this->stopAnimation();
		}
	}


}

void AnimatedSprite::draw(Graphics& graphics, int x, int y)
{
	 
	if (this->_visible) {
		SDL_Rect destinationRectangle = { 0, 0, 0, 0 };

		destinationRectangle.w = this->_sourceRect.w * globals::SPRITE_SCALE;			// scale width and height on destRect to resize the rendered image
		destinationRectangle.h = this->_sourceRect.h * globals::SPRITE_SCALE;
		destinationRectangle.x = x + this->_offsets[this->_currentAnimation].x;			
		destinationRectangle.y = y + this->_offsets[this->_currentAnimation].y;
	


		SDL_Rect sourceRect = this->_animations[this->_currentAnimation][this->_frameIndex];
		graphics.blitSurface(this->_spriteSheet, &sourceRect, &destinationRectangle);
	}
}


AnimatedSprite::~AnimatedSprite()
{
}
