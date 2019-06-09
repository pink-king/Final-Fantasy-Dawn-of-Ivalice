#include "UiItem_Inventory.h"
#include "j1Gui.h"
#include "UiItem.h"
#include "PlayerEntityManager.h"
#include "j1EntityFactory.h"
#include "j1Fonts.h"
#include "j1Render.h"
#include "j1Scene.h"
#include "j1DialogSystem.h"
#include "Brofiler/Brofiler.h"


UiItem_Inventory::UiItem_Inventory(UiItem* const parent) :UiItem(parent)
{
	this->parent = parent;
	this->guiType = GUI_TYPES::INVENTORY;
	App->scene->tab_inventory = App->gui->AddImage({ startingPos.x + 133, startingPos.y + 275 }, &tab_image, this->name, parent);
}



bool UiItem_Inventory::LoadElements(bool onlyEquipped, bool isVendor)
{

	if(createdLabels)
		despawnRubbish();


	createdLabels = true; 


	totalDeSpawnedInventoryIcons = 0;
	totalSpawnedItems = 0;
	// - - - - - - - - - - character icons
	BROFILER_CATEGORY("Inventory Load Elements", Profiler::Color::Olive);

	isVendorInventory = isVendor;

	SDL_Color c = { 255, 255, 255, 255 };

	charNameLabel = App->gui->AddLabel(App->entityFactory->player->selectedCharacterEntity->name,
		c, App->font->weekSRB18, iPoint(startingPos.x + 265, startingPos.y + 50), this);



	if (isVendorInventory)
	{
		equippedLabel = App->gui->AddLabel("PLAYER", c, App->font->weekSRB18, iPoint(startingPos.x + 30, startingPos.y + 80), this);
		bagLabel = App->gui->AddLabel("VENDOR", c, App->font->weekSRB18, iPoint(startingPos.x + 30, startingPos.y + 280), this);
		consumablesLabel = App->gui->AddLabel("VENDOR CONSUMABLES", c, App->font->weekSRB18, iPoint(startingPos.x + 500, startingPos.y + 300), this);
		consumablesLabel->hide = true;
	}
	else
	{
		equippedLabel = App->gui->AddLabel("EQUIPPED", c, App->font->weekSRB18, iPoint(startingPos.x + 30, startingPos.y + 80), this);
		bagLabel = App->gui->AddLabel("EQUIPPABLE", c, App->font->weekSRB18, iPoint(startingPos.x + 30, startingPos.y + 280), this);
		consumablesLabel = App->gui->AddLabel("CONSUMABLES", c, App->font->weekSRB18, iPoint(startingPos.x + 500, startingPos.y + 300), this);
	}


	/*if (!isVendorInventory)
	{*/
	firstSwapedCharacter = false;
		if (App->entityFactory->player->selectedCharacterEntity->character == characterName::MARCHE)
		{
			App->scene->MarcheIcon->hide = false;
			App->scene->SharaIcon->hide = true;
			App->scene->RitzIcon->hide = true;
		}

		else if (App->entityFactory->player->selectedCharacterEntity->character == characterName::SHARA)
		{
			App->scene->MarcheIcon->hide = true;
			App->scene->SharaIcon->hide = false;
			App->scene->RitzIcon->hide = true;
		}


		else if (App->entityFactory->player->selectedCharacterEntity->character == characterName::RITZ)
		{
			App->scene->MarcheIcon->hide = true;
			App->scene->SharaIcon->hide = true;
			App->scene->RitzIcon->hide = false;
		}


	
	if(isVendorInventory)
	{
		if(App->entityFactory->player->GetVendor()->firstTime)
		App->entityFactory->player->GetVendor()->generateVendorItems();

		/*App->scene->MarcheIcon->hide = true;
		App->scene->SharaIcon->hide = true;
		App->scene->RitzIcon->hide = true;*/
	}




	// - - - - - - - - - - - - - - - - - - - - - - - - - -  equipped objects   --> ONLY FOR PLAYER, NOT FOR VENDOR !!


	if (/*!isVendorInventory &&*/!App->entityFactory->player->equipedObjects.empty()) 
	{
		if (!firstSwapedCharacter)
		{
			for (std::vector<LootEntity*>::iterator iter = App->entityFactory->player->equipedObjects.begin(); iter != App->entityFactory->player->equipedObjects.end(); ++iter)
			{
				if ((*iter)->MyDescription != nullptr)
					De_______GenerateDescription((*iter), false);
			}
			firstSwapedCharacter = true;
		}

		if (firstSwapedCharacter)
		{
			std::vector<LootEntity*>::iterator iter = App->entityFactory->player->equipedObjects.begin();

			for (; iter != App->entityFactory->player->equipedObjects.end(); ++iter)
			{


				iPoint destPos = {};

				if ((*iter)->character == App->entityFactory->player->selectedCharacterEntity)  // Only load the selected character current items
				{


					// first generate description if it does not have it or if it was deleted ingame

					De_______GenerateDescription((*iter), true);


					// current weapon, armor and head have a target position each
					switch ((*iter)->GetObjectType())
					{
					case OBJECT_TYPE::WEAPON_OBJECT:
						destPos.x = startingPos.x + initialPositionsOffsets.currentWeapon.x;
						destPos.y = startingPos.y + initialPositionsOffsets.currentWeapon.y;

						(*iter)->MyDescription->myLootItemIsEquipped.weapon = true;
						break;
					case OBJECT_TYPE::ARMOR_OBJECT:
						destPos.x = startingPos.x + initialPositionsOffsets.currentArmor.x;
						destPos.y = startingPos.y + initialPositionsOffsets.currentArmor.y;

						(*iter)->MyDescription->myLootItemIsEquipped.armor = true;
						break;
					case OBJECT_TYPE::HEAD_OBJECT:
						destPos.x = startingPos.x + initialPositionsOffsets.currentHead.x;
						destPos.y = startingPos.y + initialPositionsOffsets.currentHead.y;

						(*iter)->MyDescription->myLootItemIsEquipped.head = true;
						break;


					}

					// create the icon image directly in that desired slot (aka position) 


					if (!(*iter)->MyDescription->spawnedInventoryImage)
					{
						(*iter)->MyDescription->panelWithButton->section = App->scene->lootPanelRectNoButton;
						(*iter)->MyDescription->iconImageInventory = App->gui->AddSpecialImage(destPos, &(*iter)->MyDescription->iconImage->section, this, (*iter)->entityTex, (*iter)->MyDescription);
						(*iter)->MyDescription->iconImageInventory->printFromLoot = true;
						(*iter)->MyDescription->spawnedInventoryImage = true;
						(*iter)->MyDescription->myLootItemIsEquipped.state = ACTIVE;   // lastly put the image as active (we will need it later) 
					}
					else
					{
						(*iter)->MyDescription->HideAllElements(false);
						(*iter)->MyDescription->iconImageInventory->tabbable = true;
					}



				}
				/*else
				{
				if ((*iter)->MyDescription->spawnedInventoryImage)
				{
				(*iter)->MyDescription->HideAllElements(true);
				(*iter)->MyDescription->iconImageInventory->hide = true;
				}
				}*/



			}
		}
	}



	if (!onlyEquipped)
	{
		// - - - - - - - - - - - - - - - - - - - - - - - - - -  bag objects 

		if (!isVendorInventory)
		{
			if (!App->entityFactory->player->bagObjects.empty() && swappedBag)
			{

				int i = 0;
				int j = 0;
				int k = 0;
				

				if (firstTimeSwappedBagLeft)
				{
					for (std::vector<LootEntity*>::iterator iter = App->entityFactory->player->bagObjects.begin(); iter != App->entityFactory->player->bagObjects.end(); ++iter)
					{
						if ((*iter)->MyDescription != nullptr)
							De_______GenerateDescription((*iter), false);
					}
					firstTimeSwappedBagLeft = false;
				}

				if (!firstTimeSwappedBagLeft)
				{
					for (std::vector<LootEntity*>::iterator iter = App->entityFactory->player->bagObjects.begin(); iter != App->entityFactory->player->bagObjects.end(); ++iter)
					{

						
						// first generate description if it does not have it or if it was deleted ingame

						if (k >= 15)
							break;

						De_______GenerateDescription((*iter), true);


						iPoint position(0, 0);

						if (i == 5)
						{
							i = 0;
							j++;
						}

						if (i == 0)  // first
						{

							position = { (startingPos.x + 140), (startingPos.y + 284 + j * boxSeparation.y) };


						}
						else   // the rest of elements in the row
						{
							position = { (startingPos.x + 140 + i * boxSeparation.x), (startingPos.y + 284) + j * boxSeparation.y };
						}


						(*iter)->MyDescription->panelWithButton->section = App->scene->lootPanelRectNoButton;
						if (!(*iter)->MyDescription->spawnedInventoryImage)
						{
							(*iter)->MyDescription->iconImageInventory = App->gui->AddSpecialImage(position, &(*iter)->MyDescription->iconImage->section, this, (*iter)->entityTex, (*iter)->MyDescription);
							(*iter)->MyDescription->iconImageInventory->printFromLoot = true;

							(*iter)->MyDescription->spawnedInventoryImage = true;
						}
						else
						{
							(*iter)->MyDescription->HideAllElements(false);
							(*iter)->MyDescription->iconImageInventory->tabbable = true;
						}



						i++;
						k++;

					}
					
				}
				
			}
			if (!App->entityFactory->player->bagObjects.empty() && swappedBag2)
			{

				int i = 0;
				int j = 0;
				int k = 0;
				
					// first generate description if it does not have it or if it was deleted ingame
				if (!firstTimeSwappedBag)
				{
					for (std::vector<LootEntity*>::iterator iter = App->entityFactory->player->bagObjects.begin(); iter != App->entityFactory->player->bagObjects.end(); ++iter)
					{
						if ((*iter)->MyDescription != nullptr)
							De_______GenerateDescription((*iter), false);
					}
					firstTimeSwappedBagLeft = true;
				}
					//
				if (firstTimeSwappedBagLeft)
				{
					for (std::vector<LootEntity*>::iterator iter = App->entityFactory->player->bagObjects.begin(); iter != App->entityFactory->player->bagObjects.end(); ++iter)
					{


						if (k >= 30)
							break;

						if (k >= 15)
						{

							De_______GenerateDescription((*iter), true);
							iPoint position(0, 0);

							if (i == 5)
							{
								i = 0;
								j++;
							}

							if (i == 0)  // first
							{

								position = { (startingPos.x + 140), (startingPos.y + 284 + j * boxSeparation.y) };


							}
							else   // the rest of elements in the row
							{
								position = { (startingPos.x + 140 + i * boxSeparation.x), (startingPos.y + 284) + j * boxSeparation.y };
							}


							(*iter)->MyDescription->panelWithButton->section = App->scene->lootPanelRectNoButton;
							if (!(*iter)->MyDescription->spawnedInventoryImage)
							{
								(*iter)->MyDescription->iconImageInventory = App->gui->AddSpecialImage(position, &(*iter)->MyDescription->iconImage->section, this, (*iter)->entityTex, (*iter)->MyDescription);
								(*iter)->MyDescription->iconImageInventory->printFromLoot = true;

								(*iter)->MyDescription->spawnedInventoryImage = true;
							}
							else
							{
								(*iter)->MyDescription->HideAllElements(false);
								(*iter)->MyDescription->iconImageInventory->tabbable = true;
							}



							i++;
						}
						k++;
					}

				}
			}
			//////////////////////
			if (!App->entityFactory->player->bagObjects.empty() && swappedBag3)
			{

				int i = 0;
				int j = 0;
				int k = 0;


				if (firstTimeSwappedBagLeft3)
				{
					for (std::vector<LootEntity*>::iterator iter = App->entityFactory->player->bagObjects.begin(); iter != App->entityFactory->player->bagObjects.end(); ++iter)
					{
						if ((*iter)->MyDescription != nullptr)
							De_______GenerateDescription((*iter), false);
					}
					firstTimeSwappedBagLeft3 = false;
				}

				if (!firstTimeSwappedBagLeft3)
				{
					for (std::vector<LootEntity*>::iterator iter = App->entityFactory->player->bagObjects.begin(); iter != App->entityFactory->player->bagObjects.end(); ++iter)
					{

						
						// first generate description if it does not have it or if it was deleted ingame
						if (k >= 45)
							break;

						if (k >= 30)
						{

							De_______GenerateDescription((*iter), true);
							iPoint position(0, 0);

							if (i == 5)
							{
								i = 0;
								j++;
							}

							if (i == 0)  // first
							{

								position = { (startingPos.x + 140), (startingPos.y + 284 + j * boxSeparation.y) };


							}
							else   // the rest of elements in the row
							{
								position = { (startingPos.x + 140 + i * boxSeparation.x), (startingPos.y + 284) + j * boxSeparation.y };
							}


							(*iter)->MyDescription->panelWithButton->section = App->scene->lootPanelRectNoButton;
							if (!(*iter)->MyDescription->spawnedInventoryImage)
							{
								(*iter)->MyDescription->iconImageInventory = App->gui->AddSpecialImage(position, &(*iter)->MyDescription->iconImage->section, this, (*iter)->entityTex, (*iter)->MyDescription);
								(*iter)->MyDescription->iconImageInventory->printFromLoot = true;

								(*iter)->MyDescription->spawnedInventoryImage = true;
							}
							else
							{
								(*iter)->MyDescription->HideAllElements(false);
								(*iter)->MyDescription->iconImageInventory->tabbable = true;
							}



							i++;
						}
						k++;

					}

				}

			}
			/////////////
		}
		else
		{
			if (!App->entityFactory->player->GetVendor()->vBagObjects.empty() && swappedBagVendor)
			{
				int i = 0;
				int j = 0;
				int k = 0;

				if (firstTimeSwappedBagLeftVendor)
				{
					for (std::vector<LootEntity*>::iterator iter = App->entityFactory->player->GetVendor()->vBagObjects.begin(); iter != App->entityFactory->player->GetVendor()->vBagObjects.end(); ++iter)
					{
						if ((*iter)->MyDescription != nullptr)
							De_______GenerateDescription((*iter), false);
					}
					firstTimeSwappedBagLeftVendor = false;
				}

				if (!firstTimeSwappedBagLeftVendor)
				{

					for (std::vector<LootEntity*>::iterator iter = App->entityFactory->player->GetVendor()->vBagObjects.begin(); iter != App->entityFactory->player->GetVendor()->vBagObjects.end(); ++iter)
					{


						// first generate description if it does not have it or if it was deleted ingame
						if (k >= 15)
							break;

						De_______GenerateDescription((*iter), true);


						iPoint position(0, 0);

						if (i == 5)
						{
							i = 0;
							j++;
						}

						if (i == 0)  // first
						{

							position = { (startingPos.x + 140), (startingPos.y + 284 + j * boxSeparation.y) };


						}
						else   // the rest of elements in the row
						{
							position = { (startingPos.x + 140 + i * boxSeparation.x), (startingPos.y + 284) + j * boxSeparation.y };
						}


						(*iter)->MyDescription->panelWithButton->section = App->scene->lootPanelRectNoButton;
						if (!(*iter)->MyDescription->spawnedInventoryImage)
						{
							(*iter)->MyDescription->iconImageInventory = App->gui->AddSpecialImage(position, &(*iter)->MyDescription->iconImage->section, this, (*iter)->entityTex, (*iter)->MyDescription);
							(*iter)->MyDescription->iconImageInventory->printFromLoot = true;

							(*iter)->MyDescription->spawnedInventoryImage = true;
						}
						else
						{
							(*iter)->MyDescription->HideAllElements(false);
							(*iter)->MyDescription->iconImageInventory->tabbable = true;
						}



						i++;
						k++;

					}
				}
			}
			///////////////////
			if (!App->entityFactory->player->GetVendor()->vBagObjects.empty() && swappedBag2Vendor)
			{

				int i = 0;
				int j = 0;
				int k = 0;
				
				if (firstTimeSwappedBagVendor)
				{
					for (std::vector<LootEntity*>::iterator iter = App->entityFactory->player->GetVendor()->vBagObjects.begin(); iter != App->entityFactory->player->GetVendor()->vBagObjects.end(); ++iter)
					{
						if ((*iter)->MyDescription != nullptr)
							De_______GenerateDescription((*iter), false);
					}
					firstTimeSwappedBagVendor = false;
				}
				if (!firstTimeSwappedBagVendor)
				{
					for (std::vector<LootEntity*>::iterator iter = App->entityFactory->player->GetVendor()->vBagObjects.begin(); iter != App->entityFactory->player->GetVendor()->vBagObjects.end(); ++iter)
					{
						// first generate description if it does not have it or if it was deleted ingame

						if (k >= 30)
							break;

						if (k >= 15)
						{

							De_______GenerateDescription((*iter), true);
							iPoint position(0, 0);

							if (i == 5)
							{
								i = 0;
								j++;
							}

							if (i == 0)  // first
							{

								position = { (startingPos.x + 140), (startingPos.y + 284 + j * boxSeparation.y) };


							}
							else   // the rest of elements in the row
							{
								position = { (startingPos.x + 140 + i * boxSeparation.x), (startingPos.y + 284) + j * boxSeparation.y };
							}


							(*iter)->MyDescription->panelWithButton->section = App->scene->lootPanelRectNoButton;
							if (!(*iter)->MyDescription->spawnedInventoryImage)
							{
								(*iter)->MyDescription->iconImageInventory = App->gui->AddSpecialImage(position, &(*iter)->MyDescription->iconImage->section, this, (*iter)->entityTex, (*iter)->MyDescription);
								(*iter)->MyDescription->iconImageInventory->printFromLoot = true;

								(*iter)->MyDescription->spawnedInventoryImage = true;
							}
							else
							{
								(*iter)->MyDescription->HideAllElements(false);
								(*iter)->MyDescription->iconImageInventory->tabbable = true;
							}



							i++;
						}
						k++;
					}
				}

			}
			/////////////////////////
			if (!App->entityFactory->player->GetVendor()->vBagObjects.empty() && swappedBag3Vendor)
			{

				int i = 0;
				int j = 0;
				int k = 0;


				if (firstTimeSwappedBagLeft3Vendor)
				{
					for (std::vector<LootEntity*>::iterator iter = App->entityFactory->player->GetVendor()->vBagObjects.begin(); iter != App->entityFactory->player->GetVendor()->vBagObjects.end(); ++iter)
					{
						if ((*iter)->MyDescription != nullptr)
							De_______GenerateDescription((*iter), false);
					}
					firstTimeSwappedBagLeft3Vendor = false;
				}

				if (!firstTimeSwappedBagLeft3Vendor)
				{
					for (std::vector<LootEntity*>::iterator iter = App->entityFactory->player->GetVendor()->vBagObjects.begin(); iter != App->entityFactory->player->GetVendor()->vBagObjects.end(); ++iter)
					{

						
						// first generate description if it does not have it or if it was deleted ingame
						if (k >= 45)
							break;

						if (k >= 30)
						{

							De_______GenerateDescription((*iter), true);
							iPoint position(0, 0);

							if (i == 5)
							{
								i = 0;
								j++;
							}

							if (i == 0)  // first
							{

								position = { (startingPos.x + 140), (startingPos.y + 284 + j * boxSeparation.y) };


							}
							else   // the rest of elements in the row
							{
								position = { (startingPos.x + 140 + i * boxSeparation.x), (startingPos.y + 284) + j * boxSeparation.y };
							}


							(*iter)->MyDescription->panelWithButton->section = App->scene->lootPanelRectNoButton;
							if (!(*iter)->MyDescription->spawnedInventoryImage)
							{
								(*iter)->MyDescription->iconImageInventory = App->gui->AddSpecialImage(position, &(*iter)->MyDescription->iconImage->section, this, (*iter)->entityTex, (*iter)->MyDescription);
								(*iter)->MyDescription->iconImageInventory->printFromLoot = true;

								(*iter)->MyDescription->spawnedInventoryImage = true;
							}
							else
							{
								(*iter)->MyDescription->HideAllElements(false);
								(*iter)->MyDescription->iconImageInventory->tabbable = true;
							}



							i++;
						}
						k++;

					}

				}

			}
			/////////////////////////
		}



		// TODO: Consumables: with dpad consum consumable, already define the icons in xml, just keep a a variable that holds the number of potions of each type

		// - - - - - - - - - - - - - - - - - - - - - - - - - -  consumables

		if (!isVendorInventory)
		{
			if (!App->entityFactory->player->consumables.empty())
			{
				first_potion = false;
				potion_counter = 0;
				fenix_counter = 0;
				std::vector<LootEntity*>::iterator iter = App->entityFactory->player->consumables.begin();
				for (; iter != App->entityFactory->player->consumables.end(); ++iter)
				{
					// Potions
					if ((*iter)->objectType == OBJECT_TYPE::POTIONS)
					{
						// first generate description if it does not have it or if it was deleted ingame

						De_______GenerateDescription((*iter), true);

						iPoint position_1 = { (startingPos.x + 612), (startingPos.y + 285) };

						potion_counter++;
						



						if (!(*iter)->MyDescription->spawnedInventoryImage)
						{
							(*iter)->MyDescription->panelWithButton->section = App->scene->lootPanelRectNoButton;
							(*iter)->MyDescription->iconImageInventory = App->gui->AddSpecialImage(position_1, &(*iter)->MyDescription->iconImage->section, this, (*iter)->entityTex, (*iter)->MyDescription);
							(*iter)->MyDescription->iconImageInventory->printFromLoot = true;
							(*iter)->MyDescription->spawnedInventoryImage = true;

						}
						else
						{
							(*iter)->MyDescription->HideAllElements(false);
							(*iter)->MyDescription->iconImageInventory->tabbable = true;
						}




						if (!first_label_potion)
						{
							potionLabel = App->gui->AddLabel("1", { 255, 255, 255, 255 }, App->font->knightsQuest24, consumable_positions.potion1, this);
							first_label_potion = true;
						}
						else
						{
							str_potion = "" + std::to_string(potion_counter);
							potionLabel->ChangeTextureIdle(str_potion, NULL, NULL);
						}
					}

					
					// Fenix tails
					if ((*iter)->objectType == OBJECT_TYPE::PHOENIX_TAIL)
					{
						// first generate description if it does not have it or if it was deleted ingame

						De_______GenerateDescription((*iter), true);

						iPoint position_1 = { (startingPos.x + 612), (startingPos.y + 361) };

						fenix_counter++;




						if (!(*iter)->MyDescription->spawnedInventoryImage)
						{
							(*iter)->MyDescription->panelWithButton->section = App->scene->lootPanelRectNoButton;
							(*iter)->MyDescription->iconImageInventory = App->gui->AddSpecialImage(position_1, &(*iter)->MyDescription->iconImage->section, this, (*iter)->entityTex, (*iter)->MyDescription);
							(*iter)->MyDescription->iconImageInventory->printFromLoot = true;
							(*iter)->MyDescription->spawnedInventoryImage = true;

						}
						else
						{
							(*iter)->MyDescription->HideAllElements(false);
							(*iter)->MyDescription->iconImageInventory->tabbable = true;
						}




						if (!first_label_fenix)
						{
							fenixLabel = App->gui->AddLabel("1", { 255, 255, 255, 255 }, App->font->knightsQuest24, consumable_positions.fenix_tail, this);
							first_label_fenix = true;
						}
						else
						{
							str_fenix_tail = "" + std::to_string(fenix_counter);
							fenixLabel->ChangeTextureIdle(str_fenix_tail, NULL, NULL);
						}
					}
				}
				if (potion_counter == 0 && first_label_potion)
				{
					potionLabel->to_delete = true;
					first_label_potion = false;
				}
				if (fenix_counter == 0 && first_label_fenix)
				{
					fenixLabel->to_delete = true;
					first_label_fenix = false;
				}
			}

			else
			{
				if (first_label_potion)
				{
					potionLabel->to_delete = true;
					first_label_potion = false;
				}
				if (first_label_fenix)
				{
					fenixLabel->to_delete = true;
					first_label_fenix = false;
				}
			}
		}

		else
		{
			if (!App->entityFactory->player->GetVendor()->vConsumables.empty())
			{
				first_potion = false;
				vendor_potion_counter = 0;
				std::vector<LootEntity*>::iterator iter = App->entityFactory->player->GetVendor()->vConsumables.begin();
				for (; iter != App->entityFactory->player->GetVendor()->vConsumables.end(); ++iter)
				{

					// first generate description if it does not have it or if it was deleted ingame

					De_______GenerateDescription((*iter), true);


					iPoint position_1 = { (startingPos.x + 612), (startingPos.y + 285) };

					if ((*iter)->objectType == OBJECT_TYPE::POTIONS)
					{
						vendor_potion_counter++;
					}



					if (!(*iter)->MyDescription->spawnedInventoryImage)
					{
						(*iter)->MyDescription->panelWithButton->section = App->scene->lootPanelRectNoButton;
						(*iter)->MyDescription->iconImageInventory = App->gui->AddSpecialImage(position_1, &(*iter)->MyDescription->iconImage->section, this, (*iter)->entityTex, (*iter)->MyDescription);
						(*iter)->MyDescription->iconImageInventory->printFromLoot = true;
						(*iter)->MyDescription->spawnedInventoryImage = true;

					}
					else
					{
						(*iter)->MyDescription->HideAllElements(false);
						(*iter)->MyDescription->iconImageInventory->tabbable = true;
					}




					if (!first_label_potion)
					{
						potionLabel = App->gui->AddLabel("1", { 255, 255, 255, 255 }, App->font->knightsQuest24, consumable_positions.potion1, this);
						first_label_potion = true;
					}
					else
					{
						str_potion = "" + std::to_string(vendor_potion_counter);
						potionLabel->ChangeTextureIdle(str_potion, NULL, NULL);
					}

				}
			}
			else
			{
				if (first_label_potion)
				{
					potionLabel->to_delete = true;
					first_label_potion = false;
				}
			}
		}



	}
	else
	{
		App->gui->resetHoverSwapping = false;    // when changing characters, the selected object must be reasigned
	}


	return true;
}


void UiItem_Inventory::RepositionBagItems()
{
	if (!isVendorInventory)
	{
		if (!App->entityFactory->player->bagObjects.empty())
		{
			int i = 0;
			int j = 0;
			std::vector<LootEntity*>::iterator iter = App->entityFactory->player->bagObjects.begin();

			for (; iter != App->entityFactory->player->bagObjects.end(); ++iter)
			{

				iPoint position(0, 0);

				if (i == 5)
				{
					i = 0;
					j++;
				}

				if (i == 0)  // first
				{

					position = { (startingPos.x + 140), (startingPos.y + 284 + j * boxSeparation.y) };


				}
				else   // the rest of elements in the row
				{
					position = { (startingPos.x + 140 + i * boxSeparation.x), (startingPos.y + 284) + j * boxSeparation.y };
				}

				if ((*iter)->MyDescription != nullptr)
				{
					if ((*iter)->MyDescription->iconImageInventory->tabbable)
					{
						(*iter)->MyDescription->iconImageInventory->hitBox.x = position.x;   // ignore the bag object hidden not available
						(*iter)->MyDescription->iconImageInventory->hitBox.y = position.y;
					}
				}
				i++;

			}

		}
	}
	else
	{
		if (!App->entityFactory->player->GetVendor()->vBagObjects.empty())
		{
			int i = 0;
			int j = 0;
			std::vector<LootEntity*>::iterator iter = App->entityFactory->player->GetVendor()->vBagObjects.begin();

			for (; iter != App->entityFactory->player->GetVendor()->vBagObjects.end(); ++iter)
			{

				iPoint position(0, 0);

				if (i == 5)
				{
					i = 0;
					j++;
				}

				if (i == 0)  // first
				{

					position = { (startingPos.x + 140), (startingPos.y + 284 + j * boxSeparation.y) };


				}
				else   // the rest of elements in the row
				{
					position = { (startingPos.x + 140 + i * boxSeparation.x), (startingPos.y + 284) + j * boxSeparation.y };
				}

				if ((*iter)->MyDescription != nullptr)
				{
					if ((*iter)->MyDescription->iconImageInventory->tabbable)
					{
						(*iter)->MyDescription->iconImageInventory->hitBox.x = position.x;   // ignore the bag object hidden not available
						(*iter)->MyDescription->iconImageInventory->hitBox.y = position.y;
					}
				}

				i++;


			}

		}
	}


	App->gui->selected_object = nullptr;
	App->gui->resetHoverSwapping = false;

}


void UiItem_Inventory::De_______GenerateDescription(LootEntity * ent, bool firstTime)
{
	BROFILER_CATEGORY("Inventory Generate Description", Profiler::Color::Olive);


	if (firstTime)
	{
		if (!ent->spawnedDescription)
		{
			// create a new description
			App->entityFactory->GenerateDescriptionForLootItem(ent);
			ent->MyDescription->RepositionAllElements(iPoint(staringPosition.x + 545, staringPosition.y + 30));
			ent->MyDescription->HideAllElements(false);

			ent->spawnedDescription = true;

			totalSpawnedItems++;
			LOG("_______________________________________________   total descriptions: %i", totalSpawnedItems);
		}
		else
		{
			//	ent->MyDescription->HideAllElements(false);
		}

	}
	else   // only when closing inventory, delete the description
	{
		if(ent->MyDescription!=nullptr)
		{
			if (App->gui->selected_object == ent->MyDescription->iconImageInventory)
			{
				App->gui->selected_object = nullptr;

				if (App->dialog->isDialogSequenceActive)
					App->gui->resetHoverSwapping = false;    // reset hover swapping if inventory sequence active
			}

			// delete last descr
			ent->MyDescription->DeleteEverything();
			ent->MyDescription = nullptr;


			ent->spawnedDescription = false;



			totalSpawnedItems--;
			LOG("_______________________________________________   total descriptions: %i", totalSpawnedItems);

		}
	}


}

void UiItem_Inventory::callDeleteWhenSwitchingCharacters()
{


	/*if (!isVendorInventory) {*/

		if (!App->entityFactory->player->equipedObjects.empty())
		{
			std::vector<LootEntity*>::iterator iter = App->entityFactory->player->equipedObjects.begin();

			for (; iter != App->entityFactory->player->equipedObjects.end(); ++iter)
			{

				if ((*iter)->spawnedDescription)
				{
					if ((*iter)->MyDescription->spawnedInventoryImage)
					{

						De_______GenerateDescription((*iter), false);     // when switching characters, delete de current equipped object descriptions
					}

				}
			}

		}

	//}


}

void UiItem_Inventory::makeItemNotAvailableWhenSelectedInInventoryAndSwitchingOwner(LootEntity* ent)
{


	ent->MyDescription->HideAllElements(true, false, true);
	ent->MyDescription->iconImageInventory->tabbable = false;


	App->gui->selected_object = nullptr;
	App->gui->resetHoverSwapping = false;


}




void UiItem_Inventory::De_______Equip(LootEntity* callback)
{
	iPoint destPos = {};

	BROFILER_CATEGORY("Inventory Equip", Profiler::Color::Olive);

	bool doBagScroll = false;

	bool successfulVendorToPlayer = false;

	// 1) Check that both the item that wants to be equipped and the already equipped one belong to the current player
	if (!isVendorInventory)
	{
		if (!App->dialog->isDialogSequenceActive)
		{

			std::vector<LootEntity*>::iterator iter = App->entityFactory->player->equipedObjects.begin();

			if (callback->character == App->entityFactory->player->selectedCharacterEntity)       // Search only for the selected character's current items
			{
				for (; iter != App->entityFactory->player->equipedObjects.end(); ++iter)
				{

					if ((*iter)->character == App->entityFactory->player->selectedCharacterEntity)       // Search only for the selected character's current items
					{
						if ((*iter)->MyDescription->myLootItemIsEquipped.state == ACTIVE)
						{
							if ((*iter)->GetObjectType() == callback->GetObjectType() && (*iter)->MyDescription->myLootItemIsEquipped.weapon)
							{
								//App->entityFactory->player->DesequipItem((*iter));
								(*iter)->MyDescription->myLootItemIsEquipped.weapon = false;
								(*iter)->MyDescription->myLootItemIsEquipped.state = INACTIVE;

								// 2) The de-equipped item position will be at the grid, where the new item is (spwapping positions) 
								(*iter)->MyDescription->iconImageInventory->hitBox.x = callback->MyDescription->iconImageInventory->hitBox.x;
								(*iter)->MyDescription->iconImageInventory->hitBox.y = callback->MyDescription->iconImageInventory->hitBox.y;
							}
							else if ((*iter)->GetObjectType() == callback->GetObjectType() && (*iter)->MyDescription->myLootItemIsEquipped.armor)
							{
								//App->entityFactory->player->DesequipItem((*iter));
								(*iter)->MyDescription->myLootItemIsEquipped.armor = false;
								(*iter)->MyDescription->myLootItemIsEquipped.state = INACTIVE;

								// 2) The de-equipped item position will be at the grid, where the new item is (spwapping positions) 
								(*iter)->MyDescription->iconImageInventory->hitBox.x = callback->MyDescription->iconImageInventory->hitBox.x;
								(*iter)->MyDescription->iconImageInventory->hitBox.y = callback->MyDescription->iconImageInventory->hitBox.y;
							}
							else if ((*iter)->GetObjectType() == callback->GetObjectType() && (*iter)->MyDescription->myLootItemIsEquipped.head)
							{
								//App->entityFactory->player->DesequipItem((*iter));
								(*iter)->MyDescription->myLootItemIsEquipped.head = false;
								(*iter)->MyDescription->myLootItemIsEquipped.state = INACTIVE;

								// 2) The de-equipped item position will be at the grid, where the new item is (spwapping positions) 
								(*iter)->MyDescription->iconImageInventory->hitBox.x = callback->MyDescription->iconImageInventory->hitBox.x;
								(*iter)->MyDescription->iconImageInventory->hitBox.y = callback->MyDescription->iconImageInventory->hitBox.y;
							}
						}


					}

				}
				App->entityFactory->player->EquipItem(callback);        // PLAYER TO PLAYER: when no inventory sequence is active


				// ADD NEW STATS TO THE PLAYER 
				App->scene->characterStatsItem->SetNewStats();

				if (!App->entityFactory->player->bagObjects.empty())
				{
					doBagScroll = true;
				}


			}



			//App->gui->resetHoverSwapping = false;
		}
		else
		{


			if (DoPriceCalculations(callback))
			{
				// delete item desciption so that it is not selected again
				makeItemNotAvailableWhenSelectedInInventoryAndSwitchingOwner(callback);

				// PLAYER TO VENDOR: when the inventory sequence is active: no need to check character

				if (callback->GetType() == LOOT_TYPE::EQUIPABLE)
				{
					App->entityFactory->player->RemoveItemFromBag(callback);

					App->audio->PlayFx(App->scene->sell, 0);
					if (!App->entityFactory->player->bagObjects.empty())
					{
						doBagScroll = true;
					}


				}
				else if (callback->GetType() == LOOT_TYPE::CONSUMABLE)
				{
					App->entityFactory->player->RemoveItemFromConsumables(callback);

					potion_counter--;
					App->audio->PlayFx(App->scene->sell, 0);

					if (potion_counter > 0)
					{
						str_potion = "" + std::to_string(potion_counter);
						potionLabel->ChangeTextureIdle(str_potion, NULL, NULL);

					}
					else
					{
						potionLabel->to_delete = true;
						first_label_potion = false;
					}

				}


				App->entityFactory->player->GetVendor()->EquipVendor(callback, true);
			}



		}




	}
	else // VENDOR TO PLAYER
	{
		if (callback->GetType() == LOOT_TYPE::EQUIPABLE)
		{
			std::vector<LootEntity*>::iterator iter = App->entityFactory->player->GetVendor()->vBagObjects.begin();

			for (; iter != App->entityFactory->player->GetVendor()->vBagObjects.end(); ++iter)
			{
				if ((*iter) == callback)                            // capture the vendor item
				{

					if (callback->character == App->entityFactory->player->selectedCharacterEntity)
					{



						if (!App->entityFactory->player->equipedObjects.empty())
						{

							std::vector<LootEntity*>::iterator iterPlayer = App->entityFactory->player->equipedObjects.begin();

							for (; iterPlayer != App->entityFactory->player->equipedObjects.end(); ++iterPlayer)
							{
								if ((*iterPlayer)->character == App->entityFactory->player->selectedCharacterEntity)   // check they both have same attached character
								{

									if (DoPriceCalculations(callback))
									{

										successfulVendorToPlayer = true;

										// Now, equipping from vendor is possible: remove player equipped pbject description, and equip vendor's item

										if ((*iterPlayer)->MyDescription->myLootItemIsEquipped.state == ACTIVE)
										{
											if ((*iterPlayer)->GetObjectType() == callback->GetObjectType() && (*iterPlayer)->MyDescription->myLootItemIsEquipped.weapon)
											{

												De_______GenerateDescription((*iterPlayer), false);
											}
											else if ((*iterPlayer)->GetObjectType() == callback->GetObjectType() && (*iterPlayer)->MyDescription->myLootItemIsEquipped.armor)
											{
												De_______GenerateDescription((*iterPlayer), false);
											}
											else if ((*iterPlayer)->GetObjectType() == callback->GetObjectType() && (*iterPlayer)->MyDescription->myLootItemIsEquipped.head)
											{
												De_______GenerateDescription((*iterPlayer), false);
											}
										}

									}

								}
							}
						}


						if (DoPriceCalculations(callback))
						{

							successfulVendorToPlayer = true;


							App->entityFactory->player->GetVendor()->DeEquipVendor(callback);
							App->audio->PlayFx(App->scene->purchase, 0);
							App->entityFactory->player->AddItemToTheBag(callback);


							if (!App->entityFactory->player->GetVendor()->vBagObjects.empty())   // reposition bag items if holes
							{
								doBagScroll = true;
							}


							// ADD NEW STATS TO THE PLAYER 
							App->scene->characterStatsItem->SetNewStats();


							break;

						}

					}


				}

			}

		}
		else if (callback->GetType() == LOOT_TYPE::CONSUMABLE) // todo: consumables, don't need to check anything
		{
			std::vector<LootEntity*>::iterator iter = App->entityFactory->player->GetVendor()->vConsumables.begin();

			for (; iter != App->entityFactory->player->GetVendor()->vConsumables.end(); ++iter)
			{
				if ((*iter) == callback)
				{

					if (DoPriceCalculations(callback))
					{

						// delete item desciption so that it is not selected again
						makeItemNotAvailableWhenSelectedInInventoryAndSwitchingOwner(callback);
						App->audio->PlayFx(App->scene->purchase, 0);
						App->entityFactory->player->GetVendor()->DeEquipVendor(callback);
						App->entityFactory->player->AddItemToConsumables(callback);


						vendor_potion_counter--;

						if (vendor_potion_counter > 0)
						{
							str_potion = "" + std::to_string(vendor_potion_counter);
							potionLabel->ChangeTextureIdle(str_potion, NULL, NULL);
						}
						else
						{
							potionLabel->to_delete = true;
							first_label_potion = false;
						}



						break;
					}


				}

			}
		}




	}




	if (callback->character == App->entityFactory->player->selectedCharacterEntity)
	{
		switch (callback->GetObjectType())
		{
		case OBJECT_TYPE::WEAPON_OBJECT:

			if ((!isVendorInventory && !App->dialog->isDialogSequenceActive) || successfulVendorToPlayer)           // player to player 
			{
				callback->MyDescription->myLootItemIsEquipped.weapon = true;
				callback->MyDescription->iconImageInventory->hitBox.x = startingPos.x + initialPositionsOffsets.currentWeapon.x;
				callback->MyDescription->iconImageInventory->hitBox.y = startingPos.y + initialPositionsOffsets.currentWeapon.y;
			}


			break;


		case OBJECT_TYPE::ARMOR_OBJECT:

			if ((!isVendorInventory && !App->dialog->isDialogSequenceActive) || successfulVendorToPlayer)            // player to player 
			{
				callback->MyDescription->myLootItemIsEquipped.armor = true;
				callback->MyDescription->iconImageInventory->hitBox.x = startingPos.x + initialPositionsOffsets.currentArmor.x;
				callback->MyDescription->iconImageInventory->hitBox.y = startingPos.y + initialPositionsOffsets.currentArmor.y;
			}

			break;


		case OBJECT_TYPE::HEAD_OBJECT:

			if ((!isVendorInventory && !App->dialog->isDialogSequenceActive) || successfulVendorToPlayer)           // player to player 
			{
				callback->MyDescription->myLootItemIsEquipped.head = true;
				callback->MyDescription->iconImageInventory->hitBox.x = startingPos.x + initialPositionsOffsets.currentHead.x;
				callback->MyDescription->iconImageInventory->hitBox.y = startingPos.y + initialPositionsOffsets.currentHead.y;
			}

			break;

		}


		if ((!isVendorInventory && !App->dialog->isDialogSequenceActive) || successfulVendorToPlayer)     // Only for player, switches between equipped and bag 
		callback->MyDescription->myLootItemIsEquipped.state = ACTIVE;

	}


	if (doBagScroll)
		RepositionBagItems();




}



bool UiItem_Inventory::DoPriceCalculations(LootEntity* item)
{

	bool ret = false;


	if (CheckMaxItems())
	{
		// player to vendor 

		if (!App->scene->inventoryItem->isVendorInventory)
		{
			App->entityFactory->player->gold += item->price;
			App->entityFactory->player->str_coin = std::to_string(App->entityFactory->player->gold) + " x";
			App->scene->coins_label->ChangeTextureIdle(App->entityFactory->player->str_coin, NULL, NULL);
			App->scene->coins_label_inventory->ChangeTextureIdle(App->entityFactory->player->str_coin, NULL, NULL);

			ret = true;
		}
		else	// vendor to player
		{
			if (App->entityFactory->player->gold >= item->vendorPrice)
			{
				App->entityFactory->player->gold -= item->vendorPrice;
				App->entityFactory->player->str_coin = std::to_string(App->entityFactory->player->gold) + " x";
				App->scene->coins_label->ChangeTextureIdle(App->entityFactory->player->str_coin, NULL, NULL);
				App->scene->coins_label_inventory->ChangeTextureIdle(App->entityFactory->player->str_coin, NULL, NULL);

				ret = true;
			}

		}

	}



	return ret;
}




bool UiItem_Inventory::CheckMaxItems()
{
	bool ret = false; 

	if (isVendorInventory)
	{
		if (App->entityFactory->player->bagObjects.size() < MAX_Bag_Capacity)  // check destinatiuon player bag
		{
			ret = true; 
		}
	}
	else
	{
		if (App->entityFactory->player->GetVendor()->vBagObjects.size() < MAX_Bag_Capacity)  // check destination vendor bag
		{
			ret = true;
		}
	}

	return ret; 
}
/*
void UiItem_Inventory::swapCharacterItemsWithoutSwappingCharacter(std::string name)
{
	callDeleteWhenSwitchingCharacters();


	if (name == "Marche")
	{
		App->scene->MarcheIcon->hide = false;
		App->scene->SharaIcon->hide = true;
		App->scene->RitzIcon->hide = true;
	}

	else if (name == "Shara")
	{
		App->scene->MarcheIcon->hide = true;
		App->scene->SharaIcon->hide = false;
		App->scene->RitzIcon->hide = true;
	}


	else if (name == "Ritz")
	{
		App->scene->MarcheIcon->hide = true;
		App->scene->SharaIcon->hide = true;
		App->scene->RitzIcon->hide = false;
	}



	if (!App->entityFactory->player->equipedObjects.empty())
	{
		std::vector<LootEntity*>::iterator iter = App->entityFactory->player->equipedObjects.begin();

		for (; iter != App->entityFactory->player->equipedObjects.end(); ++iter)
		{


			iPoint destPos = {};

			if ((*iter)->character->name == name)                                         // Not selected character, but hovered item's attached character
			{


				// first generate description if it does not have it or if it was deleted ingame

				De_______GenerateDescription((*iter), true);


				// current weapon, armor and head have a target position each
				switch ((*iter)->GetObjectType())
				{
				case OBJECT_TYPE::WEAPON_OBJECT:
					destPos.x = startingPos.x + initialPositionsOffsets.currentWeapon.x;
					destPos.y = startingPos.y + initialPositionsOffsets.currentWeapon.y;

					(*iter)->MyDescription->myLootItemIsEquipped.weapon = true;
					break;
				case OBJECT_TYPE::ARMOR_OBJECT:
					destPos.x = startingPos.x + initialPositionsOffsets.currentArmor.x;
					destPos.y = startingPos.y + initialPositionsOffsets.currentArmor.y;

					(*iter)->MyDescription->myLootItemIsEquipped.armor = true;
					break;
				case OBJECT_TYPE::HEAD_OBJECT:
					destPos.x = startingPos.x + initialPositionsOffsets.currentHead.x;
					destPos.y = startingPos.y + initialPositionsOffsets.currentHead.y;

					(*iter)->MyDescription->myLootItemIsEquipped.head = true;
					break;


				}

				// create the icon image directly in that desired slot (aka position) 


				if (!(*iter)->MyDescription->spawnedInventoryImage)
				{
					(*iter)->MyDescription->panelWithButton->section = App->scene->lootPanelRectNoButton;
					(*iter)->MyDescription->iconImageInventory = App->gui->AddSpecialImage(destPos, &(*iter)->MyDescription->iconImage->section, this, (*iter)->entityTex, (*iter)->MyDescription);
					(*iter)->MyDescription->iconImageInventory->printFromLoot = true;
					(*iter)->MyDescription->spawnedInventoryImage = true;
					(*iter)->MyDescription->myLootItemIsEquipped.state = ACTIVE;   // lastly put the image as active (we will need it later) 
				}
				else
				{
					(*iter)->MyDescription->HideAllElements(false);
					(*iter)->MyDescription->iconImageInventory->tabbable = true;
				}



			}
			/*else
			{
			if ((*iter)->MyDescription->spawnedInventoryImage)
			{
			(*iter)->MyDescription->HideAllElements(true);
			(*iter)->MyDescription->iconImageInventory->hide = true;
			}
			}*/



		/*}
	}
}*/

void UiItem_Inventory::despawnRubbish()
{
	charNameLabel->to_delete = true;
	equippedLabel->to_delete = true;
	bagLabel->to_delete = true;
	consumablesLabel->to_delete = true;


	createdLabels = false; 
}
