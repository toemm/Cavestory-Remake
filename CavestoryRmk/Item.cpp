#include "Item.h"
#include "Graphics.h"
#include "Rectangle.h"
#include "Player.h"

#include <memory>

Item::Item()
{
}

Item::Item(int ID, std::string itemDescription, Rectangle collRect) :
_ID(ID),
_itemDescription(itemDescription),
_collRect(collRect)
{

}

Item::~Item()
{
}



Weapon::Weapon()
{

}

Weapon::Weapon(int ID, std::string itemDescription, Rectangle collRect, WeaponProperty weaponProperty,
	Graphics& graphics, std::string filePath, int sourceX, int sourceY, int width, int height,
	float posX, float posY, float timeToUpdate) :

	Item(ID, itemDescription, collRect),
	AnimatedSprite(graphics, filePath, sourceX, sourceY, width, height, posX, posY, timeToUpdate),
	_weaponProperty(weaponProperty)
	
{

}

Consumable::Consumable()
{

}

Consumable::Consumable(int ID, std::string itemDescription, Rectangle collRect) :
Item(ID, itemDescription, collRect)
{

}


