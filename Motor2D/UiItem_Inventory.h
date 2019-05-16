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

#define staringPosition iPoint(278, 101)
#define tabOffset iPoint(15,17)
#define tabOffsetPotion iPoint(23,18)

struct elementsStartingPositionsOffsets
{
	iPoint currentWeapon = iPoint(140, 30);
	iPoint currentHead = iPoint(140, 102);
	iPoint currentArmor = iPoint(140, 178);

	iPoint potions = iPoint(100, 100);
};
struct consumableLabelPositions
{
	iPoint potion1 = iPoint(928, 407);
	iPoint fenix_tail = iPoint(928, 457);
};


class LootEntity;
class UiItem_Description;
class UiItem_Label;
class UiItem_Image;

class UiItem_Inventory : public UiItem
{

public:
	UiItem_Inventory(UiItem* const parent);

	bool LoadElements(bool onlyEquipped = false, bool isVendor = false);
	//void DoLogicSelected(LootEntity*, bool);

	void De_______Equip(LootEntity*);

	void De_______GenerateDescription(LootEntity*, bool firstTime);

	void callDeleteWhenSwitchingCharacters();

	void makeItemNotAvailableWhenSelectedInInventoryAndSwitchingOwner(LootEntity*);

	void RepositionBagItems(); 

	bool DoPriceCalculations(LootEntity* );

public: 

	bool drawTest = false;

	bool isVendorInventory = false; 
	bool swappedBag = true;
	bool swappedBag2 = false;
	bool swappedBag3 = false;
	bool firstTimeSwappedBag = false;
	bool firstTimeSwappedBagLeft = false;
	bool firstTimeSwappedBagLeft3 = false;

	bool swappedBagVendor = true;
	bool swappedBag2Vendor = false;
	bool swappedBag3Vendor = false;
	bool firstTimeSwappedBagVendor = false;
	bool firstTimeSwappedBagLeftVendor = false;
	bool firstTimeSwappedBagLeft3Vendor = false;


	elementsStartingPositionsOffsets initialPositionsOffsets;
	iPoint startingPos = { 278, 101 };

private:
	SDL_Rect tab_image = { 726,1,69,70 };
	bool first_potion = false;
	bool first_label_potion = false;
	bool first_label_fenix = false;
	int potion_counter = 1;
	int fenix_counter = 1;
	int vendor_potion_counter = 1; 
	UiItem_Label* potionLabel = nullptr;
	UiItem_Label* fenixLabel = nullptr;
	consumableLabelPositions consumable_positions;
	std::string str_potion;
	std::string str_fenix_tail;



};









#endif