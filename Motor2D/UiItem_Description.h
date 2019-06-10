#ifndef UIITEM_DESCRIPTION_H
#define UIITEM_DESCRIPTION_H

#include "UiItem.h"
#include "p2Point.h"
#include "j1Textures.h"
#include <cstdint>
#include <string.h>
#include "CharacterStats.h"
#include "j1App.h"
// #include "j1Entity.h"


class UiItem_Image;
class UiItem_Label;
class UIItem_Inventory;
class CharacterStats;

struct EquipmentStatType
{
	int HP = 0;
	int velocity = 0;
};

struct WeaponStatType
{
	int cooldown = 0;
	int defense = 0;
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




class LootEntity;


struct comparisonLabel
{
	UiItem_Label* label = nullptr;
	std::string type;               // sword, etc  
	std::string character;
	std::string text;
	int value;
};

class UiItem_Description : public UiItem
{

public:
	UiItem_Description(iPoint position, std::string itemName, const SDL_Rect* panelRect, const SDL_Rect* iconRect, float Value, EquipmentStatType variableType, uint level, LootEntity* callback, UiItem* const parent, uint price);   // for equipment
	UiItem_Description(iPoint position, std::string itemName, const SDL_Rect* panelRect, const SDL_Rect* iconRect, float Attack, float resistance, float cooldown, uint level, LootEntity* callback, UiItem* const parent, uint price);   // for weapons, right now we will print TWO variables
	UiItem_Description(iPoint position, std::string itemName, const SDL_Rect* panelRect, const SDL_Rect* iconRect, std::string effect, iPoint HPandTime, LootEntity* callback, UiItem* const parent, uint price);   // for potions
	~UiItem_Description();

	void Draw(const float& dt);   // do we need this?? I bet we don't 

	void HideAllElements(bool hide = true, bool closeInventory = false, bool buyingOrSelling = false);
	void RepositionAllElements(iPoint referencePanelPosition);
	bool ChangeComparisonLabels();
	void HideAllComparisonLabels();
	void SwitchCameraUsage();

	void DeleteEverything();

public:
	// - - - - common - - - - // 
	UiItem_Image* iconImage = nullptr;
	UiItem_Image* panelWithButton = nullptr;
	UiItem_Label* name = nullptr;
	UiItem_Label* level = nullptr;
	UiItem_Image* iconImageInventory = nullptr;


	// for weapons and armors, but not potions

	UiItem_Label* attachedCharacter = nullptr;
	std::string attachedCharacterString;

	// for weapons
	UiItem_Label* damageLabel = nullptr;
	UiItem_Label* resistanceLabel = nullptr;  // also for armor
	UiItem_Label* cooldownLabel = nullptr;

	comparisonLabel damageComparisonLabel;
	comparisonLabel resistanceComparisonLabel;  // also for armor
	comparisonLabel cooldownComparisonLabel;


	// for armors
	UiItem_Label* HPLabel = nullptr;
	UiItem_Label* VelocityLabel = nullptr;

	comparisonLabel HPComparisonLabel;
	comparisonLabel velocityComparisonLabel;


	// for potions 
	UiItem_Label* effectLabel = nullptr;


	// price

	UiItem_Label* price = nullptr;

	bool hide = false;
	bool spawnedInventoryImage = false;
	bool switchedCameraUsage = false;

	bool showedPrice = false;
	bool spawnedInGame = false; 

	uint offsetFromLoot = 1000;

	iPoint referencePanelPosition;

	// TODO: variable to_delete: description (and all items contained) are only deleted when the loot item is sold in the store


	currenEquipped myLootItemIsEquipped;


	LootEntity* callback = nullptr;

	// to capture
	float attack;
	float resistance;
	float cooldown;

	float HP;
	float velocity;

	// flag
	bool hasToCompare = true;
//	void LastHoveredCharacterStatSwapReset(); 

private:

	EquipmentStatType equipmentLootInfo;
	WeaponStatType weaponLootInfo;
	descriptionType descrType;
	bool tabOnConsumable = false;
	bool foundPoti = false;
};

#endif