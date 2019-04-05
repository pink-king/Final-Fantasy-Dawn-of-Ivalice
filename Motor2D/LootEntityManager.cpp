#include "LootEntityManager.h"
#include "j1Render.h"
#include "p2Log.h"
#include "j1Map.h"
#include "j1LootSystem.h"

LootEntityManager::LootEntityManager(iPoint pos) : j1Entity(LOOT, pos.x, pos.y, "LootItem")
{
	if (App->loot->GoldSystem)
	{
		gold = new Gold(pos.x, pos.y, App->loot->goldearned);
		lootItems.push_back(gold);
		lootList.push_back(gold);
		selectedLootEntity = gold;
		App->loot->GoldSystem = false;
	}

	//SelectedItem();
}

LootEntityManager::~LootEntityManager()
{
	delete gold;
}
//void LootEntityManager::SelectedItem()
//{
//	if (App->loot->GoldSystem)
//	{
//		lootItems.push_back(gold);
//	}
//}

bool LootEntityManager::Start()
{
	std::vector<LootEntity*>::iterator item = lootItems.begin();

	for (; item != lootItems.end(); ++item)
		(*item)->Start();

	return true;
}


bool LootEntityManager::PreUpdate()
{
	return true;
}

bool LootEntityManager::Update(float dt)
{

	//SelectLoot();
	

	position = selectedLootEntity->position;
	std::vector<LootEntity*>::iterator item = lootItems.begin();

	for (; item != lootItems.end(); ++item)
	{
		(*item)->value = selectedLootEntity->value;
	}
	selectedLootEntity->Update(dt);
	return true;
}


bool LootEntityManager::PostUpdate()
{

	position.x = App->loot->loot_posX;
	/*std::vector
	App->loot->loot_posX = App->loot->loot_pos.x;
	App->loot->loot_pos = App->map->WorldToSubtileMap;*/

	selectedLootEntity->PostUpdate();
	return true;
}


bool LootEntityManager::CleanUp()
{

	std::vector<LootEntity*>::iterator item = lootItems.begin();

	for (; item != lootItems.end(); ++item)
	{
		(*item)->CleanUp();
	}
	return true;
}

void LootEntityManager::SelectLoot()
{

	if (App->loot->GoldSystem)
	{
		gold = new Gold(position.x, position.y, App->loot->goldearned);
		lootItems.push_back(gold);
		lootList.push_back(gold);
		selectedLootEntity = gold;
		App->loot->GoldSystem = false;
	}
}