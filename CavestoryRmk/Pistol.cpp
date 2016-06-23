#include "Pistol.h"

#include "Player.h"
#include "PistolBullet.h"
#include "globals.h"

Pistol::Pistol()
{

}

Pistol::Pistol(int ID, Rectangle collRect, Graphics& graphics) :
Weapon(ID, ItemDescriptions["pistol"], collRect, WeaponProperty(), graphics,
"content/sprites/Arms.png", 0, 0, 17, 10, 0, 0, 100)
{
	this->setupAnimation();
}

void Pistol::fire(Direction dir, bool isLookingUp, bool isLookingDown)
{
	// Firing the weapon
	// play the current's ammunition caret animation at the guns right or left side
	if (isLookingUp == true) {
		dynamic_cast<PistolBullet *>(this->getAmmunition())->getCaret()->playAnimation(dir == RIGHT ? "PistolCaretUpRight" : "PistolCaretUpLeft", true);
	}
	else {
		dynamic_cast<PistolBullet *>(this->getAmmunition())->getCaret()->playAnimation(dir == RIGHT ? "PistolCaretRight" : "PistolCaretLeft", true);
	}
	dynamic_cast<PistolBullet *>(this->getAmmunition())->getCaret()->setVisible(true);

	// animate the bullet according to where the player is shooting (left, right, top)
	if (isLookingUp == true) {
		this->getAmmunition()->playAnimation(dir == RIGHT ? "BulletShootUpRight" : "BulletShootUpLeft");
	}
	else {
		this->getAmmunition()->playAnimation(dir == RIGHT ? "BulletShootRight" : "BulletShootLeft");
	}

	this->getAmmunition()->setDirection(dir);
	this->getAmmunition()->setVisible(true);
}

void Pistol::touchPlayer(Player *player, std::vector<std::shared_ptr<Item>>& items)
{
	for (int i = 0; i < items.size(); i++) {
		player->addWeapon("pistol", std::dynamic_pointer_cast<Weapon>(items[i]));
	}
}

void Pistol::animationDone(std::string currentAnimation)
{

}

void Pistol::setupAnimation()
{
	this->addAnimation(1, 3, 10, "IdleAimLeft", 17, 10, Vector2(-10, 17));
	this->addAnimation(1, 3, 20, "IdleAimRight", 17, 10, Vector2(6, 5));

	// looking up
	this->addAnimation(1, 3, 32, "IdleAimUpLeft", 16, 16, Vector2(-10, -6));
	this->addAnimation(1, 3, 48, "IdleAimUpRight", 16, 16, Vector2(-8, -6));
}

void Pistol::update(int elapsedTime)
{
	this->getAmmunition()->update(elapsedTime);
	AnimatedSprite::update(elapsedTime);
}

void Pistol::draw(Graphics& graphics, int x, int y)
{
	this->getAmmunition()->draw(graphics, x, y);
	AnimatedSprite::draw(graphics, x, y);
}