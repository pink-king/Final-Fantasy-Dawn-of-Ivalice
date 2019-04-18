#include "UiItem_Inventory.h"
#include "j1Gui.h"
#include "PlayerEntityManager.h"
#include "j1EntityFactory.h"
#include "j1Render.h"


UiItem_Inventory::UiItem_Inventory(UiItem * const parent):UiItem(parent)
{
	this->parent = parent; 
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
					position = { (startingPos.x + 56 + i * boxSeparation.x + i * boxSize), (startingPos.y + 302) + j * boxSeparation.y };
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


