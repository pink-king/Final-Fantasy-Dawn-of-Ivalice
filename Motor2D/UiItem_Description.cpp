#include "UiItem_Description.h"
#include "j1Gui.h"

UiItem_Description::UiItem_Description(iPoint position, std::string itemName, const SDL_Rect* panelRect, const SDL_Rect* iconRect, float Value, EquipmentStatType variableType, UiItem*const parent) : UiItem(position, parent)
{
	this->descrType = descriptionType::EQUIPMENT; 
	this->parent = parent; 

	// TODO

}


UiItem_Description::UiItem_Description(iPoint position, std::string itemName, const SDL_Rect* panelRect, const SDL_Rect* iconRect, float Attack, float resistance, UiItem*const parent) : UiItem(position, parent)
{

	this->descrType = descriptionType::WEAPON;
	this->parent = parent;


	iconImage = App->gui->AddImage(position, iconRect, this);   // the icon must use the loot texture atlas
	iconImage->printFromLoot = true;

	panelWithButton = App->gui->AddImage(position, panelRect, this);


	
}


void UiItem_Description::HideAllElements(bool hide)
{
		this->iconImage->hide = hide;
		this->panelWithButton->hide = hide;
		this->name->hide = hide;
		this->hide = hide;

	   
		if (this->descrType == descriptionType::WEAPON)
		{
			this->damageLabel->hide = hide; 
			this->reistanceLabel->hide = hide;

		}
		else if(this->descrType == descriptionType::EQUIPMENT)
		{

		}


}


void UiItem_Description::DeleteEverything()
{

	this->iconImage->to_delete = true;
	this->panelWithButton->to_delete = true;
	this->name->to_delete = true;

	if (this->descrType == descriptionType::WEAPON)
	{
		this->damageLabel->to_delete = true;
		this->reistanceLabel->to_delete = true;

	}
	else if (this->descrType == descriptionType::EQUIPMENT)
	{

	}

	this->to_delete = true;

}
