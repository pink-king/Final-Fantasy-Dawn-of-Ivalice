#include "LootEntityManager.h"
#include "j1Render.h"
#include "p2Log.h"



LootEntityManager::LootEntityManager(iPoint pos) : j1Entity(LOOT, pos.x, pos.y, "Gold")
{
	gold = new Gold(pos.x, pos.y, App->loot->goldearned);

	SelectedItem();
}

LootEntityManager::~LootEntityManager()
{
	delete gold;
}
void LootEntityManager::SelectedItem()
{
	if (App->loot->GoldSystem)
	{
		lootItems.push_back(gold);
	}
}

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
	selectedLootEntity->Update(dt);

	position = selectedLootEntity->position;
	std::vector<LootEntity*>::iterator item = lootItems.begin();

	for (; item != lootItems.end(); ++item)
	{
		(*item)->value = selectedLootEntity->value;
	}
	return true;
}


bool LootEntityManager::PostUpdate()
{
	
	selectedLootEntity->PostUpdate();
	return true;
}


