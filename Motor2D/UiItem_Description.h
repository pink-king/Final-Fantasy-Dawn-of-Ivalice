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



class UiItem_Description: public UiItem
{

public:
	UiItem_Description(iPoint position, std::string itemName, const SDL_Rect* panelRect, const SDL_Rect* iconRect, float Value, EquipmentStatType variableType);   // for equipment
	UiItem_Description(iPoint position, std::string itemName, const SDL_Rect* panelRect, const SDL_Rect* iconRect, float Attack, float AbilityPower);   // for weapons, right now we will print TWO variables

//	void Draw(const float& dt);   // do we need this?? I bet we don't 


public:
	UiItem_Image* iconImage = nullptr;
	UiItem_Image* panelWithButton = nullptr; 
	UiItem_Label* name = nullptr;
	

	bool hide = false; 

private: 
	iPoint position; 

private:

	EquipmentStatType equipmentLootInfo; 
	
};

#endif