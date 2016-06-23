#pragma once

#include"globals.h"

class Rectangle
{
public:
	Rectangle() {}
	Rectangle(int x, int y, int width, int height) :
		_x(x),
		_y(y),
		_width(width),
		_height(height),
		_isCollisionRect(true)
	{
	}

	const int getCenterX() const { return this->_x + this->_width / 2;  }
	const int getCenterY() const { return this->_x + this->_height / 2; }

	const int getLeft() const { return this->_x; }
	const int getRight() const { return this->_x + this->_width; }
	const int getTop() const { return this->_y; }
	const int getBottom() const { return this->_y + this->_height; }

	inline const void setX(int x) { this->_x = x; }
	inline const void setY(int y) { this->_y = y; }
	inline const void setW(int w) { this->_width = w; }
	inline const void setH(int h) { this->_height = h; }

	const int getWidth() const { return this->_width; }
	const int getHeight() const { return this->_height; }

	const int getSide(const sides::Side side) const {
		return
			side == sides::LEFT ? this->getLeft() :
			side == sides::RIGHT ? this->getLeft() :
			side == sides::TOP ? this->getBottom() :
			side == sides::BOTTOM ? this->getTop() :
			sides::NONE;
	}

	void setCollision(bool b) { this->_isCollisionRect = b; }
	const bool isCollisionRect() const { return this->_isCollisionRect; }

	// bool collidesWidth
	// Takes in another Rectangle and checks if the two are colliding
	const bool collidesWith(const Rectangle& other) const {
		return
			this->getRight() >= other.getLeft() &&
			this->getLeft() <= other.getRight() &&
			this->getTop() <= other.getBottom() &&
			this->getBottom() >= other.getTop();
	}

	const bool isValidRectangle() const {
		return this->_x >= 0 && this->_y >= 0 && this->_width >= 0 && this->_height >= 0;
	}

	const inline Rectangle& getRect() const { return *this; }

	~Rectangle() {}


private:
	int _x, _y, _width, _height;
	bool _isCollisionRect;
};

