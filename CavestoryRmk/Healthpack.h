#pragma once

#include "Item.h"

class Healthpack :
	public Consumable
{
public:
	Healthpack();
	Healthpack(int ID, std::string itemDescription, Rectangle collRect);

	virtual void touchPlayer(Player *player, std::vector<std::shared_ptr<Item>>& items) override;



private:

};