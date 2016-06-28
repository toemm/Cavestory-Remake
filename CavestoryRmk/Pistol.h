#pragma once

#include "Item.h"
#include "PistolBullet.h"

class Pistol :
	public Weapon
{

public:
	Pistol();
	Pistol(int ID, Rectangle collRect, Graphics& graphics);

	// pure virtual functs
	virtual void fire(Direction dir, bool isLookingUp, bool isLookingDown) override;
	virtual void touchPlayer(Player *player, std::vector<std::shared_ptr<Item>>& items) override;
	virtual void animationDone(std::string currentAnimation) override;
	virtual void setupAnimation() override;

	// virtual functs
	virtual void update(int elapsedTime) override;
	virtual void draw(Graphics& graphics, int x, int y) override;

private:


};