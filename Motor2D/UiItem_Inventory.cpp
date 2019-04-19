#include "UiItem_Inventory.h"
#include "j1Gui.h"
#include "UiItem.h"
#include "PlayerEntityManager.h"
#include "j1EntityFactory.h"
#include "j1Fonts.h"
#include "j1Render.h"



UiItem_Inventory::UiItem_Inventory(UiItem * const parent):UiItem(parent)
{
	this->parent = parent; 
	this->guiType = GUI_TYPES::INVENTORY;
	App->scene->tab_inventory = App->gui->AddImage({ startingPos.x + 41, startingPos.y + 285 }, &tab_image, parent);
}



bool UiItem_Inventory::LoadElements()
{
	if (!App->entityFactory->player->bagObjects.empty())
	{
		int i = 0;
		int j = 0;
	//	int k = 0;  // just for the first time; 
		std::vector<LootEntity*>::iterator iter = App->entityFactory->player->bagObjects.begin();


		for (; iter != App->entityFactory->player->bagObjects.end(); ++iter)
		{
			if (!(*iter)->MyDescription->spawnedInventoryImage)
			{
				iPoint position(0, 0);

				if (i == 5)
				{
					i = 0;
					j++;
				}

				if (i == 0)  // first
				{

					position = { (startingPos.x + 56), (startingPos.y + 302 + j * boxSeparation.y) };


				}
				else   // the rest of elements in the row
				{
					position = { (startingPos.x + 56 + i * boxSeparation.x), (startingPos.y + 302) + j * boxSeparation.y };
				}


				(*iter)->MyDescription->panelWithButton->section = App->scene->lootPanelRectNoButton;   // change to panel with no button 

				(*iter)->MyDescription->iconImageInventory = App->gui->AddSpecialImage(position, &(*iter)->MyDescription->iconImage->section, this, (*iter)->entityTex, (*iter)->MyDescription);
				(*iter)->MyDescription->iconImageInventory->printFromLoot = true;
				(*iter)->MyDescription->spawnedInventoryImage = true;

			/*	if (k == 0)
				{
					DoLogicSelected((*iter), true);
				}
				k++; */

				i++;
			}


		}

	}




	return true;
}

/*
void UiItem_Inventory::DoLogicSelected(LootEntity * ent, bool doIt)
{

	App->gui->selected_object = ent->MyDescription->iconImageInventory;
	ent->MyDescription->iconImageInventory->tabbed = true; 
	ent->MyDescription->iconImageInventory->state = HOVER; 

	if(doIt)
	ent->MyDescription->HideAllElements(false); 
	else
		ent->MyDescription->HideAllElements(true);

}*/

void UiItem_Inventory::Draw(const float & dt)
{
	/*SDL_Rect draw = { 0 }; 

	if (!drawTest)
	{
		for (int i = 0; i < MAX_Weapons_plus_Equipment; ++i)
		{
			draw.x = initialPositions.weaponsAndEquipment.x + i*boxSize;


			App->render->DrawQuad(); 
		}
	}*/


	//App->gui->ApplyTabBetweenSimilar(true); 



}


void UiItem_Inventory::De_______Equip(LootEntity * callback)
{
	iPoint destPos = {}; 


	// 1) First de-equip if there is a current equipped item of the same type 

	std::vector<LootEntity*>::iterator iter = App->entityFactory->player->equipedObjects.begin();

	for (; iter != App->entityFactory->player->equipedObjects.end(); ++iter)
	{
		if ((*iter)->GetObjectType() == OBJECT_TYPE::WEAPON_OBJECT &&  (*iter)->MyDescription->myLootItemIsEquipped.weapon)
		{
			//App->entityFactory->player->DesequipItem((*iter));
			(*iter)->MyDescription->myLootItemIsEquipped.weapon = false;
			(*iter)->MyDescription->myLootItemIsEquipped.state = INACTIVE; 

			// 2) The de-equipped item position will be at the grid, where the new item is (spwapping positions) 
			(*iter)->MyDescription->iconImageInventory->hitBox.x = callback->MyDescription->iconImageInventory->hitBox.x;
			(*iter)->MyDescription->iconImageInventory->hitBox.y = callback->MyDescription->iconImageInventory->hitBox.y;
		}
		else if ((*iter)->GetObjectType() == OBJECT_TYPE::ARMOR_OBJECT && (*iter)->MyDescription->myLootItemIsEquipped.armor)
		{
			//App->entityFactory->player->DesequipItem((*iter));
			(*iter)->MyDescription->myLootItemIsEquipped.armor = false;
			(*iter)->MyDescription->myLootItemIsEquipped.state = INACTIVE;

			// 2) The de-equipped item position will be at the grid, where the new item is (spwapping positions) 
			(*iter)->MyDescription->iconImageInventory->hitBox.x = callback->MyDescription->iconImageInventory->hitBox.x;
			(*iter)->MyDescription->iconImageInventory->hitBox.y = callback->MyDescription->iconImageInventory->hitBox.y;
		}
		else if ((*iter)->GetObjectType() == OBJECT_TYPE::HEAD_OBJECT && (*iter)->MyDescription->myLootItemIsEquipped.head)
		{
			//App->entityFactory->player->DesequipItem((*iter));
			(*iter)->MyDescription->myLootItemIsEquipped.head = false;
			(*iter)->MyDescription->myLootItemIsEquipped.state = INACTIVE;

			// 2) The de-equipped item position will be at the grid, where the new item is (spwapping positions) 
			(*iter)->MyDescription->iconImageInventory->hitBox.x = callback->MyDescription->iconImageInventory->hitBox.x;
			(*iter)->MyDescription->iconImageInventory->hitBox.y = callback->MyDescription->iconImageInventory->hitBox.y;
		}

	}


	// 3) then equip them and 4) put them in the current item slots according to the type

	App->entityFactory->player->EquipItem(callback);

	switch (callback->GetObjectType())
	{
	case OBJECT_TYPE::WEAPON_OBJECT:
		callback->MyDescription->myLootItemIsEquipped.weapon = true;                               
		callback->MyDescription->iconImageInventory->hitBox.x = startingPos.x + initialPositionsOffsets.currentWeapon.x; 
		callback->MyDescription->iconImageInventory->hitBox.y = startingPos.y + initialPositionsOffsets.currentWeapon.y;

		break; 
	case OBJECT_TYPE::ARMOR_OBJECT:
		callback->MyDescription->myLootItemIsEquipped.armor = true;
		callback->MyDescription->iconImageInventory->hitBox.x = startingPos.x + initialPositionsOffsets.currentArmor.x;
		callback->MyDescription->iconImageInventory->hitBox.y = startingPos.y + initialPositionsOffsets.currentArmor.y;

		break;

	case OBJECT_TYPE::HEAD_OBJECT:
		callback->MyDescription->myLootItemIsEquipped.head = true;
		callback->MyDescription->iconImageInventory->hitBox.x = startingPos.x + initialPositionsOffsets.currentHead.x;
		callback->MyDescription->iconImageInventory->hitBox.y = startingPos.y + initialPositionsOffsets.currentHead.y;

		break;

	}

	callback->MyDescription->myLootItemIsEquipped.state = ACTIVE;


}




