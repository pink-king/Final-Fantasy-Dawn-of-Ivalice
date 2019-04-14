#include "UiItem_Description.h"


UiItem_Description::UiItem_Description(iPoint position, std::string itemName, const SDL_Rect* panelRect, const SDL_Rect* iconRect, float Value, EquipmentStatType variableType) : UiItem(position, parent)
{
}


UiItem_Description::UiItem_Description(iPoint position, std::string itemName, const SDL_Rect* panelRect, const SDL_Rect* iconRect, float Attack, float AbilityPower) : UiItem(position, parent)
{

}



