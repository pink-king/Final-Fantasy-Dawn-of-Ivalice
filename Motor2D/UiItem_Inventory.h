#ifndef UIITEM_INVENTORY_H
#define UIITEM_INVENTORY_H

#include "UiItem.h"
#include "p2Point.h"
#include "j1Textures.h"
#include <string.h>
#include <cstdint>

#define MAX_Weapons_plus_Equipment 15 
#define MAX_Potions 3
#define MAX_Equipped 3

#define boxSize 48                   
#define boxSeparation iPoint(77,74)

#define staringPosition iPoint(274, 101)
#define tabOffset iPoint(15,17)
#define tabOffsetPotion iPoint(25,23)

struct elementsStartingPositionsOffsets
{ 
	iPoint currentWeapon = iPoint(56, 47);
	iPoint currentHead = iPoint(56, 118);
	iPoint currentArmor = iPoint(56, 194);

	iPoint potions = iPoint(100, 100);
};
struct potionLabelPositions
{
	iPoint potion1 = iPoint(970, 419);

};


class LootEntity; 
class UiItem_Description; 
class UiItem_Label;

class UiItem_Inventory : public UiItem
{

public:
	UiItem_Inventory(UiItem * const parent);

	bool LoadElements(bool onlyEquipped = false);
	//void DoLogicSelected(LootEntity*, bool);
	void Draw(const float& dt) override;

	void De_______Equip(LootEntity*);

	void De_______GenerateDescription(LootEntity*, bool firstTime);

	void callDeleteWhenSwitchingCharacters(); 

	bool drawTest = false; 



	elementsStartingPositionsOffsets initialPositionsOffsets;
	iPoint startingPos = { 274, 101 };

private:
	SDL_Rect tab_image = { 726,1,69,70 };
	bool first_potion = false;
	bool first_label_potion = false;
	int potion_counter = 1;
	UiItem_Label * potionLabel = nullptr;
	potionLabelPositions potion_positions;
	std::string str_potion;
};










#endif