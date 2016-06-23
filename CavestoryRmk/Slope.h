#pragma once

#include "globals.h"
#include "Rectangle.h"



class Slope
{
public:
	Slope();
	Slope(Vector2 p1, Vector2 p2) : _p1(p1), _p2(p2)
	{
		if ((this->_p2.x - this->_p1.x) != 0) {
			this->_slope = (float) (this->_p2.y - this->_p1.y) / (this->_p2.x - this->_p1.x);
		}
	}

	const inline float getSlope() const {
		return this->_slope;
	}

	// Check if a rect collides with a slope 
	const bool collidesWith(const Rectangle& other) const {
		return
			(other.getRight() >= this->_p2.x &&			//			 /|P1
			other.getLeft() <= this->_p1.x &&			//			/ |			
			other.getTop() <= this->_p2.y &&			//		P2 /__|
			other.getBottom() >= this->_p1.y) ||
			(other.getRight() >= this->_p1.x &&			//			/|P2
			other.getLeft() <= this->_p2.x &&			//		   / |
			other.getTop() <= this->_p1.y &&			//     P1 /__|
			other.getBottom() >= this->_p2.y) ||	
			(other.getLeft() <= this->_p1.x &&			//
			other.getRight() >= this->_p2.x &&			//         |"\"
			other.getTop() <= this->_p1.y &&			//		   | "\"
			other.getBottom() >= this->_p2.y) ||		//         |__"\"
			(other.getLeft() <= this->_p2.x &&		
			other.getRight() >= this->_p1.x &&			//
			other.getTop() <= this->_p2.y &&			//
			other.getBottom() >= this->_p1.y);			//											
	}

	const inline Vector2 getP1() const { return this->_p1;  }
	const inline Vector2 getP2() const { return this->_p2;  }



	~Slope() {}


private:
	Vector2 _p1, _p2;	// every Slope has 2 points 
	float _slope;


};

