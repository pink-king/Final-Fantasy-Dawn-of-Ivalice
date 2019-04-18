#include "UiItem_Description.h"
#include "j1Gui.h"
#include "j1Fonts.h"
#include "j1Window.h"
#include "UiItem_Inventory.h"

UiItem_Description::UiItem_Description(iPoint position, std::string itemName, const SDL_Rect* panelRect, const SDL_Rect* iconRect, float Value, EquipmentStatType variableType, uint level, UiItem*const parent) : UiItem(position, parent)
{
	this->descrType = descriptionType::EQUIPMENT; 
	this->parent = parent; 
	this->guiType = GUI_TYPES::DESCRIPTION; 
	// TODO


	// the icon image is created after creating description in loot spawning
}


UiItem_Description::UiItem_Description(iPoint position, std::string itemName, const SDL_Rect* panelRect, const SDL_Rect* iconRect, float Attack, float resistance, uint level, UiItem*const parent) : UiItem(position, parent)
{

	this->descrType = descriptionType::WEAPON;
	this->parent = parent;
	this->guiType = GUI_TYPES::DESCRIPTION;

	// common 
	panelWithButton = App->gui->AddImage(iPoint(0, 0), panelRect, this);
	panelWithButton->useCamera = false; 

	if (itemName.empty())
	{
		itemName = "Item has no name"; 
	}

    name = App->gui->AddLabel(itemName, { 155, 126, 186, 255 }, App->font->openSansBold18, iPoint(0, 0), this);
	name->useCamera = false;
	

	std::string lvlString("LVL ");
	lvlString.append(std::to_string((int)level));
	this->level = App->gui->AddLabel(lvlString, { 255, 255, 255, 255 }, App->font->openSansBold18, iPoint(0, 0), this);

	this->level->useCamera = false;

	// - - - - - - - - - - - - - - - - - - 
	std::string resString("Resistance: "); 
	resString.append(std::to_string((int)resistance));
	resistanceLabel = App->gui->AddLabel(resString, { 0, 0, 0, 255 }, App->font->openSansBold18, iPoint(0, 0), this);
	
	resistanceLabel->useCamera = false;

	std::string dmgString("Damage: ");
	dmgString.append(std::to_string((int)Attack));
    damageLabel = App->gui->AddLabel(dmgString, { 0, 0, 0, 255 }, App->font->openSansBold18, iPoint(0, 0), this);
	
	damageLabel->useCamera = false;


	// the icon image is created after creating description in loot spawning
	


	

	

}


UiItem_Description::UiItem_Description(iPoint position, std::string itemName, const SDL_Rect* panelRect, const SDL_Rect* iconRect, std::string effect, iPoint HPandTime, UiItem*const parent) : UiItem(position, parent)
{

	this->descrType = descriptionType::POTION;
	this->parent = parent;
	this->guiType = GUI_TYPES::DESCRIPTION;

	// common 
	panelWithButton = App->gui->AddImage(iPoint(0, 0), panelRect, this);
	panelWithButton->useCamera = false;

	if (itemName.empty())
	{
		itemName = "Item has no name";
	}

	name = App->gui->AddLabel(itemName, { 155, 126, 186, 255 }, App->font->openSansBold18, iPoint(0, 0), this);
	name->useCamera = false;

	// TODO: effect: 


	// the icon image is created after creating description in loot spawning


}


void UiItem_Description::Draw(const float& dt)
{

	if (spawnedInventoryImage)
	{
		if (!switchedCameraUsage)
		{
			SwitchCameraUsage();           // when ingame, descr blit speed is 1.0f, but in invetory it should be 0.0f
		}

		if (App->gui->selected_object == iconImageInventory)
		{
			HideAllElements(false); 
			RepositionAllElements(iPoint(staringPosition.x + 410, staringPosition.y + 30));
		}
		else
		{
			HideAllElements(true);
		}
	}


}


void UiItem_Description::SwitchCameraUsage()
{

	// common 

	this->iconImage->useCamera = true;
	this->panelWithButton->useCamera = true;
	this->name->useCamera = true;
	this->level->useCamera = true;
	this->useCamera = true;


	if (spawnedInventoryImage)                 // only when the inventory is closed, the duplicated icon image should be hiden 
	{
		iconImageInventory->useCamera = true;
	}
	// - - - - - - - - - - - - - 

	if (this->descrType == descriptionType::WEAPON)
	{
		this->damageLabel->useCamera = true;
		this->resistanceLabel->useCamera = true;

	}
	else if (this->descrType == descriptionType::EQUIPMENT)
	{

	}
	else if (this->descrType == descriptionType::POTION)
	{
		this->effectLabel->useCamera = true;
	}

}

void UiItem_Description::HideAllElements(bool hide, bool closeInventory)
{
		// common 
	
	    this->iconImage->hide = hide;
		this->panelWithButton->hide = hide;
		this->name->hide = hide;
		this->level->hide = hide; 
		this->hide = hide;

		
		if (closeInventory)                 // only when the inventory is closed, the duplicated icon image should be hiden 
		{
			iconImageInventory->hide = true; 
		}
		// - - - - - - - - - - - - - 
	   
		if (this->descrType == descriptionType::WEAPON)
		{
			this->damageLabel->hide = hide; 
			this->resistanceLabel->hide = hide;

		}
		else if(this->descrType == descriptionType::EQUIPMENT)
		{
			
		}
		else if (this->descrType == descriptionType::POTION)
		{
			this->effectLabel->hide = hide;
		}


}

void UiItem_Description::RepositionAllElements(iPoint referencePanelPosition)
{

	this->panelWithButton->hitBox.x = referencePanelPosition.x;
	this->panelWithButton->hitBox.y = referencePanelPosition.y;

	this->iconImage->hitBox.x = referencePanelPosition.x + 20;
	this->iconImage->hitBox.y = referencePanelPosition.y + 80;

	this->name->hitBox.x = referencePanelPosition.x + 30; 
	this->name->hitBox.y = referencePanelPosition.y + 40;

	this->level->hitBox.x = referencePanelPosition.x + 150;
	this->level->hitBox.y = referencePanelPosition.y + 180;


	if (this->descrType == descriptionType::WEAPON)
	{
		this->damageLabel->hitBox.x = referencePanelPosition.x + 90;
		this->damageLabel->hitBox.y = referencePanelPosition.y + 70;

		this->resistanceLabel->hitBox.x = referencePanelPosition.x + 90;
		this->resistanceLabel->hitBox.y = referencePanelPosition.y + 100;

	}
	else if (this->descrType == descriptionType::EQUIPMENT)
	{

	}
	else if (this->descrType == descriptionType::POTION)
	{
		// TODO
	//	this->effectLabel->
	}




	// switch description type and reposition everything

}


/*
void UiItem_Description::DeleteEverything()
{

	this->iconImage->to_delete = true;
	this->panelWithButton->to_delete = true;
	this->name->to_delete = true;
	this->level->to_delete = true;

	// TODO: delete the icon image in the inventory only if it exists
	//this->iconImageInventory->to_delete = true;


	if (this->descrType == descriptionType::WEAPON)
	{
		this->damageLabel->to_delete = true;
		this->resistanceLabel->to_delete = true;

	}
	else if (this->descrType == descriptionType::EQUIPMENT)
	{

	}
	else if (this->descrType == descriptionType::POTION)
		{
			this->effectLabel->to_delete = true;
		}

	this->to_delete = true;

}*/
