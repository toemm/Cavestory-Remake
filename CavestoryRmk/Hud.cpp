#include "Hud.h"
#include "Graphics.h"
#include "Player.h"
#include "Camera.h"


Hud::Hud()
{
}


Hud::~Hud()
{
}

Hud::Hud(Graphics& graphics)

{

	this->_healthBarSprite = Sprite(graphics, "content/sprites/TextBox.png", 0, 40, 64, 8, 35, 70);
	this->_healthNumber1 = Sprite(graphics, "content/sprites/TextBox.png", 0, 56, 8, 8, 66, 70);
	this->_currentHealthBar = Sprite(graphics, "content/sprites/TextBox.png", 0, 25, 39, 5, 83, 72);

	this->_lvWord = Sprite(graphics, "content/sprites/TextBox.png", 81, 81, 11, 7, 38, 55);
	this->_lvNumber = Sprite(graphics, "content/sprites/TextBox.png", 0, 56, 8, 8, 66, 52);
	this->_expBar = Sprite(graphics, "content/sprites/TextBox.png", 0, 72, 40, 8, 83, 52);

	this->_slash = Sprite(graphics, "content/sprites/TextBox.png", 72, 48, 8, 8, 100, 36);
	this->_dashes = Sprite(graphics, "content/sprites/TextBox.png", 81, 51, 15, 11, 132, 26);




}

void Hud::update(int elapsedTime, Player& player)
{
	// Sets the sourceRect x position to the correct rectangle with the current health number
	this->_healthNumber1.setSourceRectX(8 * player.getCurrentHealth());
	
	this->_currentHealthBar.setSourceRectW(39 * ((float) player.getCurrentHealth() / player.getMaxHealth()));
}


void Hud::draw(Graphics& graphics)
{
	this->_healthBarSprite.draw(graphics, this->_healthBarSprite.getX() + Camera::cameraRect.x, this->_healthBarSprite.getY() + Camera::cameraRect.y);
	this->_healthNumber1.draw(graphics, this->_healthNumber1.getX() + Camera::cameraRect.x, this->_healthNumber1.getY() + Camera::cameraRect.y);
	this->_currentHealthBar.draw(graphics, this->_currentHealthBar.getX() + Camera::cameraRect.x, this->_currentHealthBar.getY() + Camera::cameraRect.y);

	this->_lvWord.draw(graphics, this->_lvWord.getX() + Camera::cameraRect.x, this->_lvWord.getY() + Camera::cameraRect.y);
	this->_lvNumber.draw(graphics, this->_lvNumber.getX() + Camera::cameraRect.x, this->_lvNumber.getY() + Camera::cameraRect.y);
	this->_expBar.draw(graphics, this->_expBar.getX() + Camera::cameraRect.x, this->_expBar.getY() + Camera::cameraRect.y);

	this->_slash.draw(graphics, this->_slash.getX() + Camera::cameraRect.x, this->_slash.getY() + Camera::cameraRect.y);
	this->_dashes.draw(graphics, this->_dashes.getX() + Camera::cameraRect.x, this->_dashes.getY() + Camera::cameraRect.y);


	
}
