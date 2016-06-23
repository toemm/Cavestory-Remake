#pragma once

#include "Item.h"

class Rifle :
	public Weapon
{

public:
	Rifle();
	Rifle(int ID, std::string itemDescription, Rectangle collRect, WeaponProperty weaponProperty,
		Graphics& graphics, std::string filePath, int sourceX, int sourceY, int width, int height,
		float posX, float posY, float timeToUpdate);

	// pure virtual functs
	virtual void touchPlayer(Player *player, std::vector<std::shared_ptr<Item>>& items) override;
	virtual void animationDone(std::string currentAnimation) override;
	virtual void setupAnimation() override;

	// virtual functs
	virtual void update(int elapsedTime) override;
	virtual void draw(Graphics& graphics, int x, int y) override;



private:


};