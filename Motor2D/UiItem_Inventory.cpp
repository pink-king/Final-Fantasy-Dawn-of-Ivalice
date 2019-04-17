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
		std::vector<LootEntity*>::iterator iter = App->entityFactory->player->bagObjects.begin();
		for (; iter != App->entityFactory->player->bagObjects.end(); ++iter)
		{
			if (!(*iter)->MyDescription->spawnedInventoryImage)
			{
				
				iPoint position = { (startingPos.x + 56), (startingPos.y + 300) };
				(*iter)->MyDescription->iconImageInventory = App->gui->AddSpecialImage(position, &(*iter)->MyDescription->iconImage->section, this, (*iter)->entityTex);
				(*iter)->MyDescription->iconImageInventory->printFromLoot = true;
				(*iter)->MyDescription->spawnedInventoryImage = true;
			}
			i++;
		}
	}
	return true;
}

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

}


