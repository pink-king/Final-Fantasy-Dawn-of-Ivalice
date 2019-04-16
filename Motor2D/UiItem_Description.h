#ifndef UIITEM_DESCRIPTION_H
#define UIITEM_DESCRIPTION_H

#include "UiItem.h"
#include "p2Point.h"
#include "j1Textures.h"
#include <cstdint>
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
	EQUIPMENT
};


class UiItem_Description: public UiItem
{

public:
	UiItem_Description(iPoint position, std::string itemName, const SDL_Rect* panelRect, const SDL_Rect* iconRect, float Value, EquipmentStatType variableType, UiItem*const parent);   // for equipment
	UiItem_Description(iPoint position, std::string itemName, const SDL_Rect* panelRect, const SDL_Rect* iconRect, float Attack, float resistance, UiItem*const parent);   // for weapons, right now we will print TWO variables

//	void Draw(const float& dt);   // do we need this?? I bet we don't 

	void HideAllElements(bool hide = true); 
	void RepositionAllElements(iPoint referencePanelPosition); 

	// void DeleteEverything(); 

public:
	// - - - - common - - - - // 
	UiItem_Image* iconImage = nullptr;
	UiItem_Image* panelWithButton = nullptr; 
	UiItem_Label* name = nullptr;
	
	// for weapons
	UiItem_Label* damageLabel = nullptr; 
	UiItem_Label* resistanceLabel = nullptr;

	bool hide = false; 



	iPoint referencePanelPosition; 

	// TODO: variable to_delete: description (and all items contained) are only deleted when the loot item is sold in the store

private:

	EquipmentStatType equipmentLootInfo; 
	descriptionType descrType; 
};

#endif