#include "Rocketlauncher.h"
#include "globals.h"
#include "Player.h"

Rocketlauncher::Rocketlauncher()
{
}

Rocketlauncher::Rocketlauncher(int ID, Rectangle collRect, Graphics& graphics) :
	Weapon(ID, ItemDescriptions["rocketlauncher"], collRect, WeaponProperty(), graphics,
		"content/sprites/Arms.png", 0, 0, 24, 16, 0, 0, 100)
{
	this->setupAnimation();
}

void Rocketlauncher::fire(Direction dir, bool isLookingUp, bool isLookingDown)
{

}

void Rocketlauncher::touchPlayer(Player *player, std::vector<std::shared_ptr<Item>>& items)
{
	for (int i = 0; i < items.size(); i++) {
		player->addWeapon("rocketlauncher", std::dynamic_pointer_cast<Weapon>(items[i]));
	}
}

void Rocketlauncher::animationDone(std::string currentAnimation)
{

}

void Rocketlauncher::setupAnimation()
{
	this->addAnimation(1, 5, 0, "IdleAimLeft", 24, 16, Vector2(-25, 0));
	this->addAnimation(1, 5, 16, "IdleAimRight", 24, 16, Vector2(8, 2));
}

void Rocketlauncher::update(int elapsedTime)
{
	AnimatedSprite::update(elapsedTime);
}

void Rocketlauncher::draw(Graphics& graphics, int x, int y)
{
	AnimatedSprite::draw(graphics, x, y);
}
