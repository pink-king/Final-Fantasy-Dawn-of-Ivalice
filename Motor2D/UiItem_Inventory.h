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
#define boxSeparation iPoint(77,74)

#define staringPosition iPoint(274, 101)
#define tabOffset iPoint(15,17)

struct elementsStartingPositionsOffsets
{ 
	iPoint currentWeapon = iPoint(56, 47);
	iPoint currentHead = iPoint(56, 118);
	iPoint currentArmor = iPoint(56, 194);

	iPoint potions = iPoint(100, 100);
};


class LootEntity; 
class UiItem_Description; 

class UiItem_Inventory : public UiItem
{

public:
	UiItem_Inventory(UiItem * const parent);

	bool LoadElements();
	//void DoLogicSelected(LootEntity*, bool);
	void Draw(const float& dt) override;

	void De_______Equip(LootEntity*);

	void De_______GenerateDescription(LootEntity*, bool firstTime);

	bool drawTest = false; 



	elementsStartingPositionsOffsets initialPositionsOffsets;
	iPoint startingPos = { 274, 101 };

private:
	SDL_Rect tab_image = { 726,1,69,70 };
};










#endif