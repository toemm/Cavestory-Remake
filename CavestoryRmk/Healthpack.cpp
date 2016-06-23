#include "Healthpack.h"

Healthpack::Healthpack()
{

}

Healthpack::Healthpack(int ID, std::string itemDescription, Rectangle collRect) :
Consumable(ID, itemDescription, collRect)
{

}