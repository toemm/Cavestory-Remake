#include "Healthpack.h"
#include "Player.h"

Healthpack::Healthpack()
{

}

Healthpack::Healthpack(int ID, std::string itemDescription, Rectangle collRect) :
Consumable(ID, itemDescription, collRect)
{

}

void Healthpack::touchPlayer(Player *player, std::vector<std::shared_ptr<Item>>& items)
{
	player->gainHealth(1);

}
