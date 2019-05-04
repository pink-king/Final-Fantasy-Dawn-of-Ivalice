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
	else
	{

	}


}

void Vendor::EquipVendor(LootEntity* entityLoot)
{

	if (vBagObjects.size() < 15)
	{
		vBagObjects.push_back(entityLoot);
		App->buff->AddItemStats(entityLoot);
	}
}

void Vendor::DeEquipVendor(LootEntity* entityLoot)
{
	for (std::vector<LootEntity*>::iterator item = vBagObjects.begin(); item != vBagObjects.end(); ++item)
	{
		if (entityLoot == *item)
		{
			App->buff->RemoveItemStat(*item);
			vBagObjects.erase(item);
			break;
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
