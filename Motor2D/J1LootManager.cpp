#include "j1LootManager.h"
#include "j1Render.h"
#include "p2Log.h"
#include "j1Map.h"
#include "j1EntityFactory.h"
#include "j1BuffManager.h"
#include <random>


j1LootManager::j1LootManager() 
{
	name.assign("loot");
}


j1LootManager::~j1LootManager()
{
	
}


bool j1LootManager::Start()
{
	/*goldTex = App->tex->Load("textures/loot/Gold.png");
	potionHPTex = App->tex->Load("textures/loot/health_potion.png");*/

	LootTexture = App->tex->Load("textures/loot/loot_items.png");
	return true;

}

bool j1LootManager::PreUpdate()
{
	//WillDrop();
	return true;
}
bool j1LootManager::Update(float dt)
{
	for (std::vector<j1Entity*>::iterator item = App->entityFactory->entities.begin(); item != App->entityFactory->entities.end(); ++item)
		if (App->entityFactory->player->GetSubtilePos() == (*item)->GetSubtilePos() && (*item)->type == ENTITY_TYPE::LOOT)
		{
			if (CollectLoot((LootEntity*)(*item)))
			{
				App->entityFactory->DeleteEntityFromSubtile(*item);
				item = App->entityFactory->entities.erase(item);
				break;
			}
		}

	//LOG("lootpos %i x %i y", lootPos);

	return true;
}

//retard in the set position, when enemy dies his position is saved until the next drop (DESFASE)
iPoint j1LootManager::GetEnemySubtile(j1Entity* enemy)
{
	
	return lootPos = enemy->GetSubtilePos();
}

void j1LootManager::CreateLoot(int x , int y, std::string name)
{
	App->entityFactory->CreateEntity(ENTITY_TYPE::LOOT, x, y, name);
}

iPoint j1LootManager::SetLootPos(int x, int y)
{

	return App->map->SubTileMapToWorld(x, y);

}

int j1LootManager::GetRandomValue(int min, int max)
{
	static std::default_random_engine generator;
	std::uniform_int_distribution<int> range(min, max);
	int ret_value = range(generator);

	LOG("RANDOM VALUE %i", ret_value);
	//LOG("value %i", ret_value);

	return ret_value;
}

j1Entity * j1LootManager::CreateLootType(int x, int y)
{
	j1Entity * ret = nullptr;

	switch (WillDrop())
	{
	case LOOT_TYPE::CONSUMABLE:

		ret = new Consumable(x, y);
		break;

	case LOOT_TYPE::EQUIPABLE:
		ret = new Equipable(x, y);
		break;

	default:
		break;
	}

	return ret;
}

 LOOT_TYPE j1LootManager::WillDrop()
{
	 int randvalue = GetRandomValue(1, 100);

	 if (randvalue <= lootChance)
	 {
		 toDrop = true;
		 if (randvalue <= 15)
			 return  LOOT_TYPE::CONSUMABLE;

		 else
			 return  LOOT_TYPE::EQUIPABLE;


	 }

	 else toDrop = false;



	 return LOOT_TYPE::NO_LOOT;
}

 bool j1LootManager::CollectLoot(LootEntity* entityLoot)
 {
	 if (entityLoot->GetType() == LOOT_TYPE::EQUIPABLE)
	 {
		 if (App->entityFactory->player->equipedObjects.size() == 0)
		 {
			 App->entityFactory->player->equipedObjects.push_back(entityLoot);
			 App->buff->AddItemStats(entityLoot);
		 }
		 else
		 {
			 bool existSimilar = false;
			 std::vector<LootEntity*>::iterator item = App->entityFactory->player->equipedObjects.begin();
			 for (; item != App->entityFactory->player->equipedObjects.end(); ++item)
			 {

				 if (entityLoot->GetObjectType() == (*item)->GetObjectType() && entityLoot->character == (*item)->character)
					 existSimilar = true;
			 }

			 if (!existSimilar && App->entityFactory->player != nullptr)
			 {
				 App->entityFactory->player->equipedObjects.push_back(entityLoot);
				 App->buff->AddItemStats(entityLoot);
			 }
			 else if(existSimilar && App->entityFactory->player != nullptr)
			 {
				 App->entityFactory->player->bagObjects.push_back(entityLoot);
			 }
		 }
	 }
	 else if (entityLoot->GetType() == LOOT_TYPE::CONSUMABLE)
	 {
		 if (entityLoot->GetObjectType() == OBJECT_TYPE::POTIONS)
			 App->entityFactory->player->consumables.push_back(entityLoot);

		 else if (entityLoot->GetObjectType() == OBJECT_TYPE::GOLD)
		 {
			 App->entityFactory->player->gold += entityLoot->price;
			 entityLoot->to_delete = true;
			 return false;
		 }

	 }
	 return true;
 }

 void j1LootManager::EquipItem(LootEntity* entityLoot)
 {
	 for (std::vector<LootEntity*>::iterator item = App->entityFactory->player->bagObjects.begin(); item != App->entityFactory->player->bagObjects.end(); ++item)
	 {
		 if (entityLoot == *item)
		 {
			 App->entityFactory->player->bagObjects.erase(item);
			 break;
		 }
	 }
	 if (App->entityFactory->player->equipedObjects.size() == 0)
	 {
		 App->entityFactory->player->equipedObjects.push_back(entityLoot);
		 App->buff->AddItemStats(entityLoot);
	 }
	 else
	 {
		 for (std::vector<LootEntity*>::iterator item = App->entityFactory->player->equipedObjects.begin(); item != App->entityFactory->player->equipedObjects.end(); ++item)
		 {
			
			 if (entityLoot->GetObjectType() == (*item)->GetObjectType() && entityLoot->character == (*item)->character)
			 {
				 App->buff->RemoveItemStat(*item);
				 App->entityFactory->player->bagObjects.push_back(*item);
				 App->entityFactory->player->equipedObjects.erase(item);
				 break;
			 }
		 }
		 App->entityFactory->player->equipedObjects.push_back(entityLoot);
		 App->buff->AddItemStats(entityLoot);
	 }
 }

 void j1LootManager::DesequipItem(LootEntity* entityLoot)
 {
	 for (std::vector<LootEntity*>::iterator item = App->entityFactory->player->equipedObjects.begin(); item != App->entityFactory->player->equipedObjects.end(); ++item)
	 {
		 if (entityLoot == *item)
		 {
			 App->buff->RemoveItemStat(*item);
			 App->entityFactory->player->equipedObjects.erase(item);
			 App->entityFactory->player->bagObjects.push_back(entityLoot);
			 break;
		 }
	 }
 }

 void j1LootManager::ConsumConsumable(LootEntity* consumable, j1Entity* entity)
 {
	 for (std::vector<LootEntity*>::iterator item = App->entityFactory->player->consumables.begin(); item != App->entityFactory->player->consumables.end(); ++item)
	 {
		 if (consumable == *item)
		 {
			 for (std::vector<Buff*>::iterator iter = consumable->stats.begin(); iter != consumable->stats.end(); ++iter)
			 {
				 App->buff->CreateHealth((*iter)->GetCharacter(), (*iter)->GetValue(), 8);
			 }
			 item = App->entityFactory->player->consumables.erase(item);
			 break;
		 }
	 }
 }


