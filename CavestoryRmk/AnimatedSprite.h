/* AnimatedSprite Class
* Holds logic for animating sprites
*/

#pragma once

#include <string>
#include <map>
#include <vector>
#include <iostream>

#include "Sprite.h"
#include "globals.h"

class Graphics;

class AnimatedSprite :
	public Sprite
{
public:
	AnimatedSprite();
	AnimatedSprite(Graphics& graphics, const std::string& filePath, int sourceX, int sourceY, int width, int height,
		float posX, float posY, float timeToUpdate);


	/* void playAnimation
	* Plays the animation provided if it's not already playing
	*/
	void playAnimation(std::string animation, bool once = false);


	/* void update
	* Updates the animated sprite (timer)
	*/
	virtual void update(int elapsedTime);

	/* void draw
	* Draws the sprite to the screen
	*/
	virtual void draw(Graphics& graphics, int x, int y);

	/* void setVisible
	* Changes the visibility for the animated sprite
	*/
	void setVisible(bool visible);

	~AnimatedSprite();

protected:
	double _timeToUpdate;
	bool _currentAnimationOnce;
	std::string _currentAnimation;

	/* void addAnimation
	* Adds an animation to the map of animations for the sprite
	*/
	void addAnimation(int frames, int x, int y, std::string name, int width, int height, Vector2 offset);

	/* void resetAnimations
	* Resets all animations associated with this sprite
	*/
	void resetAnimations();

	/* void stopAnimation
	* Stops the current animation
	*/
	void stopAnimation();

	/* void animationDone
	* Logic that happens when an animation ends
	*/
	virtual void animationDone(std::string currentAnimation) = 0;	// pure virtual

	/* void setupAnimation
	* A required fct that sets up all animations for a sprite
	*/
	virtual void setupAnimation() = 0;	// pure virtual

	std::map<std::string, std::vector<SDL_Rect> > _animations;
	std::map<std::string, Vector2> _offsets;

	int _frameIndex;
	double _timeElapsed;
	bool _visible;

private:
	


};

