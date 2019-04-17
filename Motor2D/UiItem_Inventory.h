#ifndef UIITEM_INVENTORY_H
#define UIITEM_INVENTORY_H

#include "UiItem.h"
#include "p2Point.h"
#include "j1Textures.h"
#include <cstdint>

#define MAX_Weapons_plus_Equipment 15 
#define MAX_Potions 3
#define MAX_Equipped 3

#define boxSize 48                   
#define boxSeparation iPoint(30,70)

struct elementsStartingPositions
{
	iPoint weaponsAndEquipment = iPoint(100, 100); 
	iPoint currentItems = iPoint(100, 100);
	iPoint potions = iPoint(100, 100);
};

class UiItem_Inventory : public UiItem
{

public:
	UiItem_Inventory(UiItem * const parent);

	bool LoadElements();
	void Draw(const float& dt) override;


	bool drawTest = false; 



	elementsStartingPositions initialPositions; 
	iPoint startingPos = { 274, 101 };

};










#endif