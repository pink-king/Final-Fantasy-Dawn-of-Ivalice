#ifndef UIITEM_INVENTORY_H
#define UIITEM_INVENTORY_H

#include "UiItem.h"
#include "p2Point.h"
#include "j1Textures.h"
#include <string.h>
#include <cstdint>

#define MAX_Bag_Capacity 45 
#define MAX_Potions 3
#define MAX_Equipped 3

#define boxSize 48                   
#define boxSeparation iPoint(77,74)

#define staringPosition iPoint(247, 96)
#define tabOffset iPoint(8,10)
#define tabOffsetPotion iPoint(8,9)

struct elementsStartingPositionsOffsets
{
	iPoint currentWeapon = iPoint(140, 102);
	iPoint currentHead = iPoint(140, 102);
	iPoint currentArmor = iPoint(140, 178);

	iPoint potions = iPoint(100, 100);
};
struct consumableLabelPositions
{
	iPoint potion1 = iPoint(890, 407);
	iPoint fenix_tail = iPoint(890, 485);
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

	bool DoPriceCalculations(LootEntity*);

	bool CheckMaxItems(); // use when buying and selling

	/*void swapCharacterItemsWithoutSwappingCharacter(std::string characterTag);*/

	void despawnRubbish();


	void SwapVendorBag(bool newPlayer);

public: 

	bool drawTest = false;
	bool firstSwapedCharacter = false;

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


	bool createdLabels = false; 


	elementsStartingPositionsOffsets initialPositionsOffsets;
	iPoint startingPos = { 247, 96 };

	uint totalSpawnedItems = 0;
	uint totalDeSpawnedInventoryIcons = 0;

private:
	SDL_Rect tab_image = { 726,1,64,64 };
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

	std::string name = "";

	UiItem_Label* charNameLabel = nullptr;
	UiItem_Label* equippedLabel = nullptr;
	UiItem_Label* bagLabel = nullptr;
	UiItem_Label* consumablesLabel = nullptr;

};









#endif