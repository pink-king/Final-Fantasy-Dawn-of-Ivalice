#ifndef UIITEM_DESCRIPTION_H
#define UIITEM_DESCRIPTION_H

#include "UiItem.h"
#include "p2Point.h"
#include "j1Textures.h"
#include <cstdint>
#include <string.h>
// #include "j1Entity.h"


class UiItem_Image;
class UiItem_Label; 

enum EquipmentStatType
{
	HP,
	HIT_SEC,
	COOLDOWN,
	ATT,
};

enum descriptionType
{
	WEAPON,
	EQUIPMENT,
	POTION
};

enum InventoryIconSate
{
	ACTIVE,
	INACTIVE
};

struct currenEquipped
{
	bool head = false; 
	bool armor = false; 
	bool weapon = false; 
	InventoryIconSate state = INACTIVE; 
};





struct comparisonLabel
{
	UiItem_Label* label = nullptr; 
	std::string type;               // sword, etc  
	std::string character; 
	std::string text;
	uint value; 
};

class LootEntity;
class UiItem_Description: public UiItem
{

public:
	UiItem_Description(iPoint position, std::string itemName, const SDL_Rect* panelRect, const SDL_Rect* iconRect, float Value, EquipmentStatType variableType, uint level, LootEntity* callback,  UiItem*const parent);   // for equipment
	UiItem_Description(iPoint position, std::string itemName, const SDL_Rect* panelRect, const SDL_Rect* iconRect, float Attack, float resistance, uint level, LootEntity* callback,  UiItem*const parent);   // for weapons, right now we will print TWO variables
	UiItem_Description(iPoint position, std::string itemName, const SDL_Rect* panelRect, const SDL_Rect* iconRect, std::string effect, iPoint HPandTime, LootEntity* callback, UiItem*const parent);   // for weapons, right now we will print TWO variables

    void Draw(const float& dt);   // do we need this?? I bet we don't 

	void HideAllElements(bool hide = true, bool closeInventory = false); 
	void RepositionAllElements(iPoint referencePanelPosition); 
	bool ChangeComparisonLabels(); 
	void SwitchCameraUsage(); 

	// void DeleteEverything(); 

public:
	// - - - - common - - - - // 
	UiItem_Image* iconImage = nullptr;
	UiItem_Image* panelWithButton = nullptr; 
	UiItem_Label* name = nullptr;
	UiItem_Label* level = nullptr;
	UiItem_Image* iconImageInventory = nullptr; 

	// for weapons and armors, but not potions

	UiItem_Label* attachedCharacter = nullptr;
	
	// for weapons
	UiItem_Label* damageLabel = nullptr; 
	UiItem_Label* resistanceLabel = nullptr;

	comparisonLabel damageComparisonLabel; 
	comparisonLabel resistanceComparisonLabel;  // this one will be used in armors too for defense
	// for potions 
	UiItem_Label* effectLabel = nullptr; 

	bool hide = false; 
	bool spawnedInventoryImage = false; 
	bool switchedCameraUsage = false; 

	uint offsetFromLoot = 1000; 

	iPoint referencePanelPosition; 

	// TODO: variable to_delete: description (and all items contained) are only deleted when the loot item is sold in the store


	currenEquipped myLootItemIsEquipped; 


	LootEntity* callback = nullptr; 

	// to capture
	float attack; 
	float resistance; 

	// flag
	bool hasToCompare = true; 
private:

	EquipmentStatType equipmentLootInfo; 
	descriptionType descrType; 
	std::string name_object;
};

#endif