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
	App->scene->tab_inventory = App->gui->AddImage({ startingPos.x + 126, startingPos.y + 267 }, &tab_image, parent);
}



bool UiItem_Inventory::LoadElements(bool onlyEquipped, bool isVendor)
{
	// - - - - - - - - - - character icons
	BROFILER_CATEGORY("Inventory Load Elements", Profiler::Color::Olive);

	isVendorInventory = isVendor;

	if (!isVendorInventory)
	{

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


	}
	else
	{
		App->entityFactory->player->GetVendor()->generateVendorItems();

		App->scene->MarcheIcon->hide = true;
		App->scene->SharaIcon->hide = true;
		App->scene->RitzIcon->hide = true;
	}




	// - - - - - - - - - - - - - - - - - - - - - - - - - -  equipped objects   --> ONLY FOR PLAYER, NOT FOR VENDOR !!


	if (!isVendorInventory && !App->entityFactory->player->equipedObjects.empty())
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



	if (!onlyEquipped)
	{
		// - - - - - - - - - - - - - - - - - - - - - - - - - -  bag objects 

		if (!isVendorInventory)
		{
			if (!App->entityFactory->player->bagObjects.empty())
			{

				int i = 0;
				int j = 0;
				std::vector<LootEntity*>::iterator iter = App->entityFactory->player->bagObjects.begin();

				for (; iter != App->entityFactory->player->bagObjects.end(); ++iter)
				{
					// first generate description if it does not have it or if it was deleted ingame

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


					// first generate description if it does not have it or if it was deleted ingame

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

			}
		}



		// TODO: Consumables: with dpad consum consumable, already define the icons in xml, just keep a a variable that holds the number of potions of each type

		// - - - - - - - - - - - - - - - - - - - - - - - - - -  consumables

		if (!isVendorInventory)
		{
			if (!App->entityFactory->player->consumables.empty())
			{
				first_potion = false;
				potion_counter = 0;
				std::vector<LootEntity*>::iterator iter = App->entityFactory->player->consumables.begin();
				for (; iter != App->entityFactory->player->consumables.end(); ++iter)
				{

					// first generate description if it does not have it or if it was deleted ingame

					De_______GenerateDescription((*iter), true);


					iPoint position_1 = { (startingPos.x + 623), (startingPos.y + 285) };

					if (dynamic_cast<Consumable*>(*iter)->consumableType == CONSUMABLE_TYPE::POTION)
					{
						potion_counter++;
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
						potionLabel = App->gui->AddLabel("1", { 255, 255, 255, 255 }, App->font->openSansBold18, potion_positions.potion1, this);
						first_label_potion = true;
					}
					else
					{
						str_potion = "" + std::to_string(potion_counter);
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


					iPoint position_1 = { (startingPos.x + 623), (startingPos.y + 284) };

					if (dynamic_cast<Consumable*>(*iter)->consumableType == CONSUMABLE_TYPE::POTION)
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
						potionLabel = App->gui->AddLabel("1", { 255, 255, 255, 255 }, App->font->openSansBold18, potion_positions.potion1, this);
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

				if ((*iter)->MyDescription->iconImageInventory->tabbable)
				{
					(*iter)->MyDescription->iconImageInventory->hitBox.x = position.x;   // ignore the bag object hidden not available
					(*iter)->MyDescription->iconImageInventory->hitBox.y = position.y;
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

				if ((*iter)->MyDescription->iconImageInventory->tabbable)
				{
					(*iter)->MyDescription->iconImageInventory->hitBox.x = position.x;   // ignore the bag object hidden not available
					(*iter)->MyDescription->iconImageInventory->hitBox.y = position.y;
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
			ent->MyDescription->RepositionAllElements(iPoint(staringPosition.x + 410, staringPosition.y + 30));
			ent->MyDescription->HideAllElements(false);

			ent->spawnedDescription = true;

			LOG("_______________________________________________   spawned description");
		}
		else
		{
			//	ent->MyDescription->HideAllElements(false);
		}

	}
	else   // only when closing inventory, delete the description
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



		LOG("_______________________________________________   Deleted description");


	}


}

void UiItem_Inventory::callDeleteWhenSwitchingCharacters()
{


	if (!isVendorInventory) {

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

	}


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


				if (!App->entityFactory->player->bagObjects.empty())
				{
					doBagScroll = true;
				}


			}



			//App->gui->resetHoverSwapping = false;
		}
		else
		{

			// delete item desciption so that it is not selected again
			makeItemNotAvailableWhenSelectedInInventoryAndSwitchingOwner(callback);

			// PLAYER TO VENDOR: when the inventory sequence is active: no need to check character

			if (callback->GetType() == LOOT_TYPE::EQUIPABLE)
			{
				App->entityFactory->player->RemoveItemFromBag(callback);


				if (!App->entityFactory->player->bagObjects.empty())
				{
					doBagScroll = true;
				}


			}
			else if (callback->GetType() == LOOT_TYPE::CONSUMABLE)
			{
				App->entityFactory->player->RemoveItemFromConsumables(callback);

				potion_counter--;

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
	else // VENDOR TO PLAYER
	{
		if (callback->GetType() == LOOT_TYPE::EQUIPABLE)
		{
			std::vector<LootEntity*>::iterator iter = App->entityFactory->player->GetVendor()->vBagObjects.begin();

			for (; iter != App->entityFactory->player->GetVendor()->vBagObjects.end(); ++iter)
			{
				if ((*iter) == callback)
				{
					if ((*iter)->character == App->entityFactory->player->selectedCharacterEntity)       // Search only for the selected character's current items
					{

						// delete item desciption so that it is not selected again
						makeItemNotAvailableWhenSelectedInInventoryAndSwitchingOwner(callback);



						App->entityFactory->player->GetVendor()->DeEquipVendor(callback);
						App->entityFactory->player->AddItemToTheBag(callback);


						if (!App->entityFactory->player->GetVendor()->vBagObjects.empty())   // reposition bag items if holes
						{
							doBagScroll = true;
						}


						break;
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

					// delete item desciption so that it is not selected again
					makeItemNotAvailableWhenSelectedInInventoryAndSwitchingOwner(callback);

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




	if (callback->character == App->entityFactory->player->selectedCharacterEntity)
	{
		switch (callback->GetObjectType())
		{
		case OBJECT_TYPE::WEAPON_OBJECT:

			if (!isVendorInventory && !App->dialog->isDialogSequenceActive)           // player to player 
			{
				callback->MyDescription->myLootItemIsEquipped.weapon = true;
				callback->MyDescription->iconImageInventory->hitBox.x = startingPos.x + initialPositionsOffsets.currentWeapon.x;
				callback->MyDescription->iconImageInventory->hitBox.y = startingPos.y + initialPositionsOffsets.currentWeapon.y;
			}


			break;


		case OBJECT_TYPE::ARMOR_OBJECT:

			if (!isVendorInventory && !App->dialog->isDialogSequenceActive)           // player to player 
			{
				callback->MyDescription->myLootItemIsEquipped.armor = true;
				callback->MyDescription->iconImageInventory->hitBox.x = startingPos.x + initialPositionsOffsets.currentArmor.x;
				callback->MyDescription->iconImageInventory->hitBox.y = startingPos.y + initialPositionsOffsets.currentArmor.y;
			}

			break;


		case OBJECT_TYPE::HEAD_OBJECT:

			if (!isVendorInventory && !App->dialog->isDialogSequenceActive)           // player to player 
			{
				callback->MyDescription->myLootItemIsEquipped.head = true;
				callback->MyDescription->iconImageInventory->hitBox.x = startingPos.x + initialPositionsOffsets.currentHead.x;
				callback->MyDescription->iconImageInventory->hitBox.y = startingPos.y + initialPositionsOffsets.currentHead.y;
			}

			break;

		}

		if (!isVendorInventory && !App->dialog->isDialogSequenceActive)    // Only for player, switches between equipped and bag 
			callback->MyDescription->myLootItemIsEquipped.state = ACTIVE;

	}


	if (doBagScroll)
		RepositionBagItems();




}
