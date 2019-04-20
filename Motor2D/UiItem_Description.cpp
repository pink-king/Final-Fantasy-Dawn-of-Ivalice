#include "UiItem_Description.h"
#include "j1Gui.h"
#include "j1Fonts.h"
#include "j1Window.h"
#include "UiItem_Inventory.h"
#include "j1Scene.h"
#include "j1Input.h"
#include "PlayerEntityManager.h"
#include "j1EntityFactory.h"
#include "LootEntity.h"

UiItem_Description::UiItem_Description(iPoint position, std::string itemName, const SDL_Rect* panelRect, const SDL_Rect* iconRect, float Value, EquipmentStatType variableType, uint level, LootEntity* callback, UiItem*const parent) : UiItem(position, parent)
{
	this->callback = callback; 

	this->resistance = Value; 

	this->descrType = descriptionType::EQUIPMENT; 
	this->parent = parent; 
	this->guiType = GUI_TYPES::DESCRIPTION; 
	// TODO


	// the icon image is created after creating description in loot spawning


	// add only resistance comparison label
}


UiItem_Description::UiItem_Description(iPoint position, std::string itemName, const SDL_Rect* panelRect, const SDL_Rect* iconRect, float Attack, float resistance, uint level, LootEntity* callback, UiItem*const parent) : UiItem(position, parent)
{
	this->callback = callback;

	this->attack = Attack; 
	this->resistance = resistance;   // needed for comparison labels

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
	std::string resString("RES: "); 
	resString.append(std::to_string((int)resistance));
	resistanceLabel = App->gui->AddLabel(resString, { 0, 0, 0, 255 }, App->font->openSansBold18, iPoint(0, 0), this);
	
	resistanceLabel->useCamera = false;

	std::string dmgString("DMG: ");
	dmgString.append(std::to_string((int)Attack));
    damageLabel = App->gui->AddLabel(dmgString, { 0, 0, 0, 255 }, App->font->openSansBold18, iPoint(0, 0), this);
	
	damageLabel->useCamera = false;


	// the icon image is created after creating description in loot spawning
	


	// attached character name
	std::string characterString; 

	
	if (callback->equipableType == EQUIPABLE_TYPE::SWORD)
	{
		characterString = "Marche";

		damageComparisonLabel.character = "Marche"; 
		damageComparisonLabel.type = "sword"; 
		
		resistanceComparisonLabel.character = "Marche";
		resistanceComparisonLabel.type = "sword";

	}
	else if (callback->equipableType == EQUIPABLE_TYPE::ROD)
	{
		characterString = "Ritz";

		resistanceComparisonLabel.character = "Ritz";
		resistanceComparisonLabel.type = "rod";
	}
	else if (callback->equipableType == EQUIPABLE_TYPE::BOW)
	{
		characterString = "Shara";

		resistanceComparisonLabel.character = "Shara";
		resistanceComparisonLabel.type = "bow";
	}

	this->attachedCharacter = App->gui->AddLabel(characterString, { 200, 200, 200, 255 }, App->font->openSansBold18, iPoint(0, 0), this);
	attachedCharacter->useCamera = false;


	// the label with the "+6" etc

	damageComparisonLabel.text = " ";
	damageComparisonLabel.label = App->gui->AddLabel(" ", { 0, 0, 0, 255 }, App->font->openSansBold18, iPoint(0, 0), this);
	damageComparisonLabel.label->useCamera = false;


	resistanceComparisonLabel.text = " ";
	resistanceComparisonLabel.label = App->gui->AddLabel(" ", { 0, 0, 0, 255 }, App->font->openSansBold18, iPoint(0, 0), this);
	resistanceComparisonLabel.label->useCamera = false;

}


UiItem_Description::UiItem_Description(iPoint position, std::string itemName, const SDL_Rect* panelRect, const SDL_Rect* iconRect, std::string effect, iPoint HPandTime, LootEntity* callback, UiItem*const parent) : UiItem(position, parent)
{
	this->callback = callback;
	this->descrType = descriptionType::POTION;
	this->parent = parent;
	this->guiType = GUI_TYPES::DESCRIPTION;
	this->name_object = itemName;

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


	std::string hpString("Cures ");
	hpString.append(std::to_string((int)HPandTime.x));
	hpString.append(" HP in ");
	hpString.append(std::to_string((int)HPandTime.y));
	hpString.append(" sec");

	effectLabel = App->gui->AddLabel(hpString, { 0, 0, 0, 255 }, App->font->openSansBold18, iPoint(0, 0), this);
	effectLabel->useCamera = false; 


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

		if (App->gui->selected_object == iconImageInventory )
		{
			if (App->scene->inventory->enable)             // shpw description
			{
				if (this->name_object == "potion_1")
				{
					App->scene->tab_inventory->hitBox.x = App->gui->selected_object->hitBox.x - 25;
					App->scene->tab_inventory->hitBox.y = App->gui->selected_object->hitBox.y - 23;
				}
				else
				{
					App->scene->tab_inventory->hitBox.x = App->gui->selected_object->hitBox.x - tabOffset.x;
					App->scene->tab_inventory->hitBox.y = App->gui->selected_object->hitBox.y - tabOffset.y;
				}
				
				HideAllElements(false);
				RepositionAllElements(iPoint(staringPosition.x + 410, staringPosition.y + 30));
				
				if (App->input->GetControllerButton(SDL_CONTROLLER_BUTTON_A) == KEY_DOWN)
				{
					if (myLootItemIsEquipped.state == INACTIVE)                                  // only call (de)equip if the item is not already active
					{
						App->scene->inventoryItem->De_______Equip(this->callback);  
					}
					
				}

				if(hasToCompare)
				ChangeComparisonLabels();    // "+3 dmg", "+4def ect

			
			}
			else                                        // hide description ingame
			{
				

				HideAllElements(true);
			}

		
		}
		else
		{
			HideAllElements(true);

			hasToCompare = true;  // reset comparison label
		}
	}



}


bool UiItem_Description::ChangeComparisonLabels()
{

	LOG(" --------------------------------------------    Compared items"); 

	bool ret = false;

	if (!App->entityFactory->player->equipedObjects.empty())
	{

		if (this->descrType == descriptionType::WEAPON)
		{
			std::vector<LootEntity*>::iterator lootItem = App->entityFactory->player->equipedObjects.begin();

			for (; lootItem != App->entityFactory->player->equipedObjects.end(); ++lootItem)
			{
				if ((*lootItem) != this->callback)
				{



					float attack = 0.0f;
					float resistance = 0.0f;

					if ((*lootItem)->GetObjectType() == OBJECT_TYPE::WEAPON_OBJECT)
					{

						if (App->entityFactory->player->selectedCharacterEntity == App->entityFactory->player->GetMarche()
							&& this->damageComparisonLabel.character == "Marche")
						{

							std::vector<Buff*>::iterator iter = (*lootItem)->stats.begin();

							for (; iter != (*lootItem)->stats.end(); ++iter)
							{
								if ((*iter)->GetRol() == ROL::ATTACK_ROL)
								{
									attack = (*iter)->GetValue();
								}
								else if ((*iter)->GetRol() == ROL::DEFENCE_ROL)
								{
									resistance = (*iter)->GetValue();
								}

							}

							this->resistanceComparisonLabel.value = (int)(this->resistance - resistance);    // diff between new item and current
							this->damageComparisonLabel.value = (int)(this->attack - attack);

							std::string dmgString = "+ ";
							dmgString.append(std::to_string((int)this->damageComparisonLabel.value));

							SDL_Color destColor = { 0, 0, 0, 255 };
							if (this->damageComparisonLabel.value > 0)
							{
								destColor = { 0, 255, 0, 255 };
							}
							else if (this->damageComparisonLabel.value <= 0)
							{
								destColor = { 255, 0, 0, 255 };
							}
						/*	else
							{
								destColor = { 0, 0, 0, 0 };
							}*/

							this->damageComparisonLabel.label->ChangeTextureIdle(dmgString, &destColor, App->font->openSansBold18);

							std::string resString = "+ ";
							resString.append((std::to_string((int)this->resistanceComparisonLabel.value)));

							destColor = { 0, 0, 0, 255 };
							if (this->resistanceComparisonLabel.value > 0)
							{
								destColor = { 0, 255, 0, 255 };
							}
							else if (this->resistanceComparisonLabel.value <= 0)
							{
								destColor = { 255, 0, 0, 255 };
							}
							/*else
							{
								destColor = { 0, 0, 0, 0 };
							}*/


							this->resistanceComparisonLabel.label->ChangeTextureIdle(resString, &destColor, App->font->openSansBold18);


							ret = true;
						}
						/*else if (App->entityFactory->player->selectedCharacterEntity == App->entityFactory->player->GetRitz()
							&& this->damageComparisonLabel.character == "Ritz")
						{


						}
						else if (App->entityFactory->player->selectedCharacterEntity == App->entityFactory->player->GetShara()
							&& this->damageComparisonLabel.character == "Shara")
						{


						}*/
					}




				}
				
			
			}
		}
		/*else if (this->descrType == descriptionType::EQUIPMENT)
	    {
   
	    }*/
	}
		

	hasToCompare = false;


	return ret; 

}

	



void UiItem_Description::SwitchCameraUsage()
{

	// common 

	this->iconImage->useCamera = true;
	this->panelWithButton->useCamera = true;
	this->name->useCamera = true;
	this->useCamera = true;


	if (spawnedInventoryImage)                 // only when the inventory is closed, the duplicated icon image should be hiden 
	{
		iconImageInventory->useCamera = true;
	}
	// - - - - - - - - - - - - - 

	if (this->descrType == descriptionType::WEAPON)
	{
		this->level->useCamera = true;
		this->damageLabel->useCamera = true;
		this->resistanceLabel->useCamera = true;
		this->attachedCharacter->useCamera = true; 

		this->damageComparisonLabel.label->useCamera = true; 
		this->resistanceComparisonLabel.label->useCamera = true;
	}
	else if (this->descrType == descriptionType::EQUIPMENT)
	{
		// this->level->useCamera = true;
		//this->attachedCharacter->useCamera = true;
		// this->resistanceComparisonLabel.label->useCamera = true;
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
		this->hide = hide;

		
		if (closeInventory)                 // only when the inventory is closed, the duplicated icon image should be hiden 
		{
			iconImageInventory->hide = true; 
		}
		// - - - - - - - - - - - - - 
	   
		if (this->descrType == descriptionType::WEAPON)
		{
			this->level->hide = hide;
			this->damageLabel->hide = hide; 
			this->resistanceLabel->hide = hide;
			this->attachedCharacter->hide = hide;

			this->damageComparisonLabel.label->hide = hide;
			this->resistanceComparisonLabel.label->hide = hide;
		}
		else if(this->descrType == descriptionType::EQUIPMENT)
		{
			//this->level->hide = hide;
			// this->attachedCharacter->hide = hide;
			// this->resistanceComparisonLabel.label->hide = hide;
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

	if (this->descrType != descriptionType::POTION)
	{
		this->iconImage->hitBox.x = referencePanelPosition.x + 20;   // The icon image in potions should be repositioned
		this->iconImage->hitBox.y = referencePanelPosition.y + 80;
	}



	int destNamePosX = referencePanelPosition.x + this->panelWithButton->section.w / 2 - this->name->textureDimensions.x / 2; 
	int offset = 10; 

	this->name->hitBox.x = destNamePosX - offset;
	this->name->hitBox.y = referencePanelPosition.y + 40;

	if (this->descrType == descriptionType::WEAPON || this->descrType == descriptionType::EQUIPMENT)
	{
		this->level->hitBox.x = referencePanelPosition.x + 150;
		this->level->hitBox.y = referencePanelPosition.y + 180;

		this->attachedCharacter->hitBox.x = referencePanelPosition.x + 30;
		this->attachedCharacter->hitBox.y = referencePanelPosition.y + 180;
	}

	if (this->descrType == descriptionType::WEAPON)
	{	

		this->damageLabel->hitBox.x = referencePanelPosition.x + 90;
		this->damageLabel->hitBox.y = referencePanelPosition.y + 70;

		this->resistanceLabel->hitBox.x = referencePanelPosition.x + 90;
		this->resistanceLabel->hitBox.y = referencePanelPosition.y + 100;


		this->damageComparisonLabel.label->hitBox.x = referencePanelPosition.x + 170;
		this->damageComparisonLabel.label->hitBox.y = referencePanelPosition.y + 70;
    	this->resistanceComparisonLabel.label->hitBox.x = referencePanelPosition.x + 170;
		this->resistanceComparisonLabel.label->hitBox.y = referencePanelPosition.y + 100;

	}
	else if (this->descrType == descriptionType::EQUIPMENT)
	{
		/*this->level->hitBox.x = referencePanelPosition.x + 150;
		this->level->hitBox.y = referencePanelPosition.y + 180;*/

		/*this->resistanceComparisonLabel.label->hitBox.x = ;
		this->resistanceComparisonLabel.label->hitBox.y = ;*/
	}
	else if (this->descrType == descriptionType::POTION)
	{
		// TODO: tweak this 

	    this->effectLabel->hitBox.x = referencePanelPosition.x + 30;
		this->effectLabel->hitBox.y = referencePanelPosition.y + 180;


		int offset = 10; 
		int destIconPosX = referencePanelPosition.x + this->panelWithButton->section.w / 2 - this->iconImage->section.w * this->iconImage->scaleFactor / 2;
		this->iconImage->hitBox.x = destIconPosX - offset;
		this->iconImage->hitBox.y = referencePanelPosition.y + 100;
	}




	// switch description type and reposition everything

}


/*
void UiItem_Description::DeleteEverything()
{

// TODO: put the lootentity pointer to nullptr ??? careful; 

	this->iconImage->to_delete = true;
	this->panelWithButton->to_delete = true;
	this->name->to_delete = true;


	// TODO: delete the icon image in the inventory only if it exists
	//this->iconImageInventory->to_delete = true;


	if (this->descrType == descriptionType::WEAPON)
	{
		this->level->to_delete = true;
		this->damageLabel->to_delete = true;
		this->resistanceLabel->to_delete = true;
		this->attachedCharacter->to_delete = true; 


		this->damageComparisonLabel.label->to_delete = true; 
		this->resistanceComparisonLabel.label->to_delete = true;
	}
	else if (this->descrType == descriptionType::EQUIPMENT)
	{
		this->level->to_delete = true;
		this->attachedCharacter->to_delete = true;

		this->resistanceComparisonLabel.label->to_delete = true;
	}
	else if (this->descrType == descriptionType::POTION)
		{
			this->effectLabel->to_delete = true;
		}

	this->to_delete = true;

}*/
