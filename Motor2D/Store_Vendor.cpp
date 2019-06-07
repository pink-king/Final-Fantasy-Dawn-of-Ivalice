#include "Store_Vendor.h"
#include "j1EntityFactory.h"
#include "j1BuffManager.h"


void Vendor::generateVendorItems(bool playerLvlUp)
{

	if (firstTime)
	{
		numberOfEquip = baseEquipables;   // beginning
		numberOfPot = baseCosumables;
	}
	
	if (playerLvlUp)
	{
		if (!firstTime)
		{
			numberOfEquip = numberItemsLvlUp;   // level up 
			numberOfPot = baseCosumables;
		}
		else
		{
			numberOfEquip = baseEquipables + numberItemsLvlUp * (App->entityFactory->player->level-1);     // if he levels up before openning inventory first time
			numberOfPot = baseCosumables;
		}
	}

	if (firstTime || playerLvlUp)
	{
		if (playerLvlUp)           // get rid of obsolete objects
		{
			for (auto& LootEntity : vBagObjects)
			{
				App->buff->RemoveItemStat(LootEntity);

				App->scene->inventoryItem->De_______GenerateDescription(LootEntity, false);
			}
			vBagObjects.clear();
		}

		for (int i = 1; i <= numberOfEquip; ++i)
		{
			j1Entity* equipable = App->entityFactory->CreateLootType(0, 0, LOOT_TYPE::EQUIPABLE);
			App->entityFactory->LoadLootData((LootEntity*)equipable, App->config);
			EquipVendor((LootEntity*)equipable);

		}

		if (vConsumables.size() < 10)
		{
			for (int i = 1; i <= numberOfPot; ++i)
			{
				j1Entity* consumable = DBG_NEW Consumable(0, 0, OBJECT_TYPE::POTIONS, CONSUMABLE_TYPE::POTION);
				App->entityFactory->LoadLootData((LootEntity*)consumable, App->config);
				EquipVendor((LootEntity*)consumable);

			}

		}

		if(firstTime)
		firstTime = false;
	}




}

void Vendor::EquipVendor(LootEntity* entityLoot, bool fromPlayer)
{

	
		if (entityLoot->GetType() == LOOT_TYPE::EQUIPABLE)
		{
			vBagObjects.push_back(entityLoot);
			
		}
		else if (entityLoot->GetType() == LOOT_TYPE::CONSUMABLE)
		{
			vConsumables.push_back(entityLoot);
		}
		
		if(!fromPlayer)
		App->buff->AddItemStats(entityLoot);   // if vendor recieves item from player, it already has stats


}

void Vendor::DeEquipVendor(LootEntity* entityLoot)
{
	if (entityLoot->GetType() == LOOT_TYPE::EQUIPABLE)
	{

		for (std::vector<LootEntity*>::iterator item = vBagObjects.begin(); item != vBagObjects.end(); ++item)
		{
			if (entityLoot == *item)
			{
				//App->buff->RemoveItemStat(*item);   // do we need to remove items stats? NO; because it goes to player
				vBagObjects.erase(item);
				break; 
			
			}
		}

	}
	else if (entityLoot->GetType() == LOOT_TYPE::CONSUMABLE)
	{

		for (std::vector<LootEntity*>::iterator item = vConsumables.begin(); item != vConsumables.end(); ++item)
		{
			if (entityLoot == *item)
			{
				//App->buff->RemoveItemStat(*item);   // do we need to remove items stats? NO; because it goes to player
				vConsumables.erase(item);
				break; 
			}
		}
	}
	

}


void Vendor::cleanUp()
{

	std::vector<LootEntity*>::iterator iter = vBagObjects.begin();
	for (; iter != vBagObjects.end(); ++iter)
	{
		delete *iter;
		*iter = nullptr;
	}
	vBagObjects.clear();

	std::vector<LootEntity*>::iterator iter2 = vConsumables.begin();
	for (; iter2 != vConsumables.end(); ++iter2)
	{
		delete *iter2;
		*iter2 = nullptr;
	}
	vConsumables.clear();

}
