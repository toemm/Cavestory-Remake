#pragma once

#include "AnimatedSprite.h"
#include "globals.h"
#include "Rectangle.h"

class Graphics;

class Caret :
	public AnimatedSprite
{

public:
	Caret(Graphics& graphics, const std::string& filePath, int sourceX, int sourceY, int width, int height,
		float posX, float posY, float timeToUpdate) :
		AnimatedSprite(graphics, filePath, sourceX, sourceY, width, height, posX, posY, timeToUpdate) {}

	const Vector2 getWallOffset() const { return this->_wallOffset; }
	void setWallOffset(Vector2 off) { this->_wallOffset = off; }

private:
	Vector2 _wallOffset;

};



class PistolCaret :
	public Caret
{
public:
	PistolCaret(Graphics& graphics);

	// virtual functs
	virtual void update(int elapsedTime) override;
	virtual void draw(Graphics& graphics, int x, int y) override;

	// pure virtual functs
	virtual void animationDone(std::string currentAnimation) override;
	virtual void setupAnimation() override;


private:

};