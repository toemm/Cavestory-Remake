#pragma once

#include "Rectangle.h"
#include "AnimatedSprite.h"
#include "AnimatedTile.h"
#include "Ammunition.h"

#include <string>
#include <memory>

class Player;
class Graphics;

class Item
{
public:
	Item();
	Item(int ID, std::string itemDescription, Rectangle collRect);

	virtual ~Item();

	virtual void touchPlayer(Player *player, std::vector<std::shared_ptr<Item>>& items) = 0;

	const Rectangle inline getCollRect() const { return this->_collRect; }
	const long inline getId() const { return this->_ID; }
	const std::string inline getItemDescription() const { return this->_itemDescription; }
	const std::string inline getItemName() const { return this->_itemName; }

protected:
	int _ID;

private:

	std::string _itemName;
	std::string _itemDescription;
	Rectangle _collRect;
};



struct WeaponProperty {
	std::string damageType;
	short level;
	int damage;
	int maxCapacity;
};



class Weapon :
	public Item,
	public AnimatedSprite

{
public:
	Weapon();
	Weapon(int ID, std::string itemDescription, Rectangle collRect, WeaponProperty weaponProperty,
		Graphics& graphics, std::string filePath, int sourceX, int sourceY, int width, int height,
		float posX, float posY, float timeToUpdate);
	virtual ~Weapon() = default;

	// pure virtual functs
	virtual void fire(Direction direction, bool isLookingUp, bool isLookingDown) = 0;

	const WeaponProperty inline getWeaponProperty() const { return this->_weaponProperty; }
	inline void addAmmunition(Ammunition *ammo) { this->_ammo = ammo; }
	inline Ammunition *getAmmunition() const { return this->_ammo; }

private:
	WeaponProperty _weaponProperty;
	Ammunition *_ammo;
	
};


class Consumable :
	public Item
{
public:
	Consumable();
	Consumable(int ID, std::string itemDescription, Rectangle collRect);


private:




};

