#include "Rifle.h"

#include "Player.h"

Rifle::Rifle()
{

}

Rifle::Rifle(int ID, std::string itemDescription, Rectangle collRect, WeaponProperty weaponProperty,
	Graphics& graphics, std::string filePath, int sourceX, int sourceY, int width, int height,
	float posX, float posY, float timeToUpdate) :
	Weapon(ID, itemDescription, collRect, weaponProperty, graphics, filePath, sourceX, sourceY, width, height, posX, posY, timeToUpdate)
{
	this->setupAnimation();
}


void Rifle::touchPlayer(Player *player, std::vector<std::shared_ptr<Item>>& items)
{
	for (int i = 0; i < items.size(); i++) {
		player->addWeapon("rifle", std::dynamic_pointer_cast<Weapon>(items[i]));
	}
}

void Rifle::animationDone(std::string currentAnimation)
{

}

void Rifle::setupAnimation()
{
	this->addAnimation(1, 12, 0, "IdleAimLeft", 24, 16, Vector2(-13, 0));
	this->addAnimation(1, 12, 16, "IdleAimRight", 24, 16, Vector2(-3, 0));
}

void Rifle::update(int elapsedTime)
{
	AnimatedSprite::update(elapsedTime);
}

void Rifle::draw(Graphics& graphics, int x, int y)
{
	AnimatedSprite::draw(graphics, x, y);
}
