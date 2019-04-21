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


UiItem_Description::UiItem_Description(iPoint position, std::string itemName, const SDL_Rect* panelRect, const SDL_Rect* iconRect, float Value, EquipmentStatType variableType, uint level, LootEntity* callback, UiItem* const parent) : UiItem(position, parent)
{
	this->callback = callback; 

	this->resistance = Value; 

	this->descrType = descriptionType::EQUIPMENT; 
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


	std::string resString("DEF: ");
	resString.append(std::to_string((int)Value));
	resistanceLabel = App->gui->AddLabel(resString, { 0, 0, 0, 255 }, App->font->openSansBold18, iPoint(0, 0), this);

	resistanceLabel->useCamera = false;


	// the icon image is created after creating description in loot spawning


	// attached character name
	std::string characterString;


	if (callback->equipableType == EQUIPABLE_TYPE::ARMOR)
	{
		characterString = "Marche";

		resistanceComparisonLabel.character = "Marche";
		resistanceComparisonLabel.type = "armor";

	}
	else if (callback->equipableType == EQUIPABLE_TYPE::MANTLE)
	{
		characterString = "Ritz";

		resistanceComparisonLabel.character = "Ritz";
		resistanceComparisonLabel.type = "mantle";
	}
	else if (callback->equipableType == EQUIPABLE_TYPE::VEST)
	{
		characterString = "Shara";

		resistanceComparisonLabel.character = "Shara";
		resistanceComparisonLabel.type = "vest";
	}

	this->attachedCharacter = App->gui->AddLabel(characterString, { 200, 200, 200, 255 }, App->font->openSansBold18, iPoint(0, 0), this);
	attachedCharacter->useCamera = false;


	// the label with the "+6" etc

	resistanceComparisonLabel.text = " ";
	resistanceComparisonLabel.label = App->gui->AddLabel(" ", { 0, 0, 0, 255 }, App->font->openSansBold18, iPoint(0, 0), this);
	resistanceComparisonLabel.label->useCamera = false;




	// hp, velocity or both depending on type; 

	this->equipmentLootInfo = variableType; 



	if (this->equipmentLootInfo.HP != 666)
	{
		std::string HPString("HP: ");
		HPString.append(std::to_string((int)variableType.HP));


		this->HPLabel = App->gui->AddLabel(HPString, { 0, 0, 0, 255 }, App->font->openSansBold18, iPoint(0, 0), this);
		this->HPLabel->useCamera = false;

		this->equipmentLootInfo.spawnedHP = true; 
	}
	if (this->equipmentLootInfo.velocity != 666)
	{
		std::string VelocityString("VEL: ");
		VelocityString.append(std::to_string((int)variableType.velocity));

		this->VelocityLabel = App->gui->AddLabel(VelocityString, { 0, 0, 0, 255 }, App->font->openSansBold18, iPoint(0, 0), this);
		this->VelocityLabel->useCamera = false;

		this->equipmentLootInfo.spawnedVecloty = true; 
	}

}


UiItem_Description::UiItem_Description(iPoint position, std::string itemName, const SDL_Rect* panelRect, const SDL_Rect* iconRect, float Attack, float resistance, uint level, LootEntity* callback, UiItem* const parent) : UiItem(position, parent)
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

		damageComparisonLabel.character = "Ritz";
		damageComparisonLabel.type = "rod";

		resistanceComparisonLabel.character = "Ritz";
		resistanceComparisonLabel.type = "rod";
	}
	else if (callback->equipableType == EQUIPABLE_TYPE::BOW)
	{
		characterString = "Shara";

		damageComparisonLabel.character = "Shara";
		damageComparisonLabel.type = "bow";

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


UiItem_Description::UiItem_Description(iPoint position, std::string itemName, const SDL_Rect* panelRect, const SDL_Rect* iconRect, std::string effect, iPoint HPandTime, LootEntity* callback, UiItem* const parent) : UiItem(position, parent)
{
	this->callback = callback;
	this->descrType = descriptionType::POTION;
	this->parent = parent;
	this->guiType = GUI_TYPES::DESCRIPTION;
	this->name_object = "potion_1";

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
	// generate description if the wasn't one 

	if (!App->scene->inventory->enable && spawnedInventoryImage)                       // if inventory is no longer enabled, delete description
		App->scene->inventoryItem->De_______GenerateDescription(this->callback, false);

	if (spawnedInventoryImage)
	{

		if (!switchedCameraUsage && App->scene->inventory->enable)
		{
			SwitchCameraUsage();           // when ingame, descr blit speed is 1.0f, but in invetory it should be 0.0f

			switchedCameraUsage = true;
		}

		if (App->scene->inventory->enable)             // shpw description
		{

			if (App->gui->selected_object == iconImageInventory)
			{


				if (this->name_object == "potion_1")
				{
					App->scene->tab_inventory->hitBox.x = App->gui->selected_object->hitBox.x - tabOffsetPotion.x;
					App->scene->tab_inventory->hitBox.y = App->gui->selected_object->hitBox.y - tabOffsetPotion.y;
				}
				else
				{
					App->scene->tab_inventory->hitBox.x = App->gui->selected_object->hitBox.x - tabOffset.x;
					App->scene->tab_inventory->hitBox.y = App->gui->selected_object->hitBox.y - tabOffset.y;
				}

				HideAllElements(false);
				RepositionAllElements(iPoint(staringPosition.x + 410, staringPosition.y + 20));

				if (App->input->GetControllerButton(SDL_CONTROLLER_BUTTON_A) == KEY_DOWN)
				{
					if (myLootItemIsEquipped.state == INACTIVE)                                  // only call (de)equip if the item is not already active
					{
						App->scene->inventoryItem->De_______Equip(this->callback);
					}

				}
				if (App->input->GetControllerButton(SDL_CONTROLLER_BUTTON_LEFTSHOULDER) == KEY_DOWN || App->input->GetControllerButton(SDL_CONTROLLER_BUTTON_RIGHTSHOULDER) == KEY_DOWN)
				{
					App->gui->resetHoverSwapping = false;
					App->scene->inventoryItem->LoadElements();
				}

				if (hasToCompare)
					ChangeComparisonLabels();    // "+3 dmg", "+4def ect




			}
			else
			{
				HideAllElements(true);

				hasToCompare = true;  // reset comparison label
			}
		}



	}



}


bool UiItem_Description::ChangeComparisonLabels()
{

	LOG(" --------------------------------------------    Compared items");

	bool ret = false;

	if (!App->entityFactory->player->equipedObjects.empty())
	{
			std::vector<LootEntity*>::iterator lootItem = App->entityFactory->player->equipedObjects.begin();

			for (; lootItem != App->entityFactory->player->equipedObjects.end(); ++lootItem)
			{
				if ((*lootItem) != this->callback)  // do not compare with itself
				{

					float attack = 0.0f;
					float resistance = 0.0f;

					float HP = 666.0f; 
					float velocity = 666.0f; 


					SDL_Color destColor = { 0, 0, 0, 255 };

					if (((*lootItem)->GetObjectType() == OBJECT_TYPE::WEAPON_OBJECT && this->descrType == descriptionType::WEAPON)
						|| 
						((*lootItem)->GetObjectType() == OBJECT_TYPE::HEAD_OBJECT && this->descrType == descriptionType::EQUIPMENT)
						||
						((*lootItem)->GetObjectType() == OBJECT_TYPE::ARMOR_OBJECT && this->descrType == descriptionType::EQUIPMENT)
						)
					{

						if ((App->entityFactory->player->selectedCharacterEntity == App->entityFactory->player->GetMarche()   // right now, only resistance comparion label is checked
							&& this->resistanceComparisonLabel.character == "Marche")

							|| (App->entityFactory->player->selectedCharacterEntity == App->entityFactory->player->GetRitz()
								&& this->resistanceComparisonLabel.character == "Ritz")

							|| (App->entityFactory->player->selectedCharacterEntity == App->entityFactory->player->GetShara()
								&& this->resistanceComparisonLabel.character == "Shara"))
						{

							std::vector<Buff*>::iterator iter = (*lootItem)->stats.begin();

							if (this->descrType == descriptionType::WEAPON)
							{
								for (; iter != (*lootItem)->stats.end(); ++iter)    // capture att and def 
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


								this->damageComparisonLabel.value = (int)(this->attack - attack);

								std::string dmgString = "";
								

								if (this->damageComparisonLabel.value > 0)
								{
									dmgString.append("+"); 
									destColor = { 0, 255, 0, 255 };
								}
								else if (this->damageComparisonLabel.value <= 0)
								{
									if (this->damageComparisonLabel.value == 0)
									{
										dmgString.append("+");
									}
									destColor = { 255, 0, 0, 255 };
								}

								dmgString.append(std::to_string((int)this->damageComparisonLabel.value));

								this->damageComparisonLabel.label->ChangeTextureIdle(dmgString, &destColor, App->font->openSansBold18);

							}
							else
							{
								for (; iter != (*lootItem)->stats.end(); ++iter)   // capture def and other 2 possible rols
								{
									if ((*iter)->GetRol() == ROL::DEFENCE_ROL)
									{
										resistance = (*iter)->GetValue();
									}
									else if ((*iter)->GetRol() == ROL::HEALTH)
									{
										HP = (*iter)->GetValue();
									}
									else if ((*iter)->GetRol() == ROL::VELOCITY)
									{
										velocity = (*iter)->GetValue();
									}

								}
							}
					
							// they both have resistance 

							this->resistanceComparisonLabel.value = (int)(this->resistance - resistance);    // diff between new item and current

							std::string resString = "";
							

							destColor = { 0, 0, 0, 255 };
							if (this->resistanceComparisonLabel.value > 0)
							{
								resString.append("+"); 
								destColor = { 0, 255, 0, 255 };
							}
							else if (this->resistanceComparisonLabel.value <= 0)
							{
								if (this->resistanceComparisonLabel.value == 0)
								{
									resString.append("+");
								}
								destColor = { 255, 0, 0, 255 };
							}
						
							resString.append((std::to_string((int)this->resistanceComparisonLabel.value)));

							this->resistanceComparisonLabel.label->ChangeTextureIdle(resString, &destColor, App->font->openSansBold18);


							ret = true;
						}

					}

				}

			}
	
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
	      this->level->useCamera = true;
		  this->attachedCharacter->useCamera = true;
		  this->resistanceLabel->useCamera = true;
		
		  this->resistanceComparisonLabel.label->useCamera = true;   // ADD HP LABEL AND VELOCITY LABEL
	      
		   
		  if (this->equipmentLootInfo.spawnedHP)
		  {
			  HPLabel->useCamera = true; 
		  }
		  if (this->equipmentLootInfo.spawnedVecloty)
		  {
			  this->VelocityLabel->useCamera = true; 
		  }
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
	else if (this->descrType == descriptionType::EQUIPMENT)
	{
		 this->level->hide = hide;
		this->attachedCharacter->hide = hide;
		this->resistanceLabel->hide = hide;
		
	    this->resistanceComparisonLabel.label->hide = hide;   // ADD HP LABEL AND VELOCITY LABEL
	
		if (this->equipmentLootInfo.spawnedHP)
		{
			HPLabel->hide = hide;
		}
		if (this->equipmentLootInfo.spawnedVecloty)
		{
			this->VelocityLabel->hide = hide;
		}

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
		this->resistanceLabel->hitBox.x = referencePanelPosition.x + 90;
		this->resistanceLabel->hitBox.y = referencePanelPosition.y + 70;
		
		this->resistanceComparisonLabel.label->hitBox.x = referencePanelPosition.x + 170;    // ADD HP LABEL AND VELOCITY LABEL
		this->resistanceComparisonLabel.label->hitBox.y = referencePanelPosition.y + 70;




		if (this->equipmentLootInfo.spawnedHP && this->equipmentLootInfo.spawnedVecloty)
		{
			HPLabel->hitBox.x = referencePanelPosition.x + 90;
			HPLabel->hitBox.y = referencePanelPosition.y + 100;

			VelocityLabel->hitBox.x = referencePanelPosition.x + 90;
			VelocityLabel->hitBox.y = referencePanelPosition.y + 130;
		}
		else if (this->equipmentLootInfo.spawnedHP && !this->equipmentLootInfo.spawnedVecloty)
		{
			HPLabel->hitBox.x = referencePanelPosition.x + 90;
			HPLabel->hitBox.y = referencePanelPosition.y + 100;
		}
		else if (!this->equipmentLootInfo.spawnedHP && this->equipmentLootInfo.spawnedVecloty)
		{
			VelocityLabel->hitBox.x = referencePanelPosition.x + 90;
			VelocityLabel->hitBox.y = referencePanelPosition.y + 100;
		}
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



void UiItem_Description::DeleteEverything()
{

	// TODO: put the lootentity pointer to nullptr ??? careful; 

	App->gui->destroyElement(this->iconImage);
	App->gui->destroyElement(this->panelWithButton);
	App->gui->destroyElement(this->name);

	// TODO: delete the icon image in the inventory only if it exists
	//this->iconImageInventory->to_delete = true;

	if (spawnedInventoryImage)
	{
		App->gui->destroyElement(this->iconImageInventory);
	}

	if (this->descrType == descriptionType::WEAPON)
	{

		App->gui->destroyElement(this->level);
		App->gui->destroyElement(this->damageLabel);
		App->gui->destroyElement(this->resistanceLabel);
		App->gui->destroyElement(this->attachedCharacter);

		App->gui->destroyElement(this->damageComparisonLabel.label);
		App->gui->destroyElement(this->resistanceComparisonLabel.label);
	}
	else if (this->descrType == descriptionType::EQUIPMENT)
	{
		App->gui->destroyElement(this->level);
		App->gui->destroyElement(this->attachedCharacter);
		App->gui->destroyElement(this->resistanceLabel);

		App->gui->destroyElement(this->resistanceComparisonLabel.label);  // ADD HP AND VELOCITY LABELS


		if (this->equipmentLootInfo.spawnedHP)
		{
			App->gui->destroyElement(this->HPLabel); 
		}

		if (this->equipmentLootInfo.spawnedVecloty)
		{
			App->gui->destroyElement(this->VelocityLabel);
		}
	}
	else if (this->descrType == descriptionType::POTION)
	{
		App->gui->destroyElement(this->effectLabel);
	}

	App->gui->destroyElement(this);

}
