#include "Store_Vendor.h"
#include "j1EntityFactory.h"
#include "j1BuffManager.h"


void Vendor::generateVendorItems()
{

	if (firstTime)
	{
		for (int i = 1; i <= 10; ++i)
		{
			j1Entity* equipable = App->entityFactory->CreateLootType(0, 0, LOOT_TYPE::EQUIPABLE);
			App->entityFactory->LoadLootData((LootEntity*)equipable, App->config);
			EquipVendor((LootEntity*)equipable);

		}

		for (int i = 1; i <= 3; ++i)
		{
			j1Entity* consumable = App->entityFactory->CreateLootType(0, 0, LOOT_TYPE::CONSUMABLE);
			App->entityFactory->LoadLootData((LootEntity*)consumable, App->config);
			EquipVendor((LootEntity*)consumable);

		}

		firstTime = false;
	}
	else                     // don't generate any more if player hasn't leveled up
	{

	}


}

void Vendor::EquipVendor(LootEntity* entityLoot, bool fromPlayer)
{

	
		if (entityLoot->GetType() == LOOT_TYPE::EQUIPABLE)
		{

			if (vBagObjects.size() < 15)
			{
				vBagObjects.push_back(entityLoot);
			}
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
