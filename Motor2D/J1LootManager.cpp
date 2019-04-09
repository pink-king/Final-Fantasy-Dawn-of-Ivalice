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

bool j1LootManager::Awake(pugi::xml_node& config)
{
	bool ret = true;
		int i = 0;
		
		health = config.child("potion").child("health").attribute("value").as_int();

		//use vectors
		for (auto node : config.child("Money").children("gold"))
		{
			//goldValue[i] = node.attribute("value").as_int();
		//gold_id[i] = node.attribute("id").as_int();
			++i;
		}
		i = 0;
		for (auto node : config.child("equipable").children("equipment"))
		{
			//EquipmentID[i] = node.attribute("id").as_int();
			//LOG("equipment %i added", EquipmentID[i]);
			++i;
		}
		return ret;
	
}

bool j1LootManager::Start()
{

	return true;

}

bool j1LootManager::PreUpdate()
{
	return true;
}
bool j1LootManager::Update(float dt)
{
	for(std::vector<j1Entity*>::iterator item = App->entityFactory->entities.begin(); item != App->entityFactory->entities.end(); ++item)
		if (App->entityFactory->player->GetSubtilePos() == (*item)->GetSubtilePos() && (*item)->type == ENTITY_TYPE::LOOT)
		{
			if (CollectLoot((LootEntity*)(*item)))
			{
				App->entityFactory->DeleteEntityFromSubtile(*item);
				item = App->entityFactory->entities.erase(item);
				break;
			}
		}

	LOG("lootpos %i x %i y", lootPos);

	return true;
}


//iPoint j1LootManager::GetPlayerSubtile(PlayerEntity* player)
//{
//	return player->GetSubtilePos();
//}

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

j1Entity* j1LootManager::CreateLootType(int x, int y)
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
	//LoadItemData(object);
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

				if (entityLoot->GetObjectType() == (*item)->GetObjectType())
					existSimilar = true;
			}

			if (!existSimilar)
			{
				App->entityFactory->player->equipedObjects.push_back(entityLoot);
				App->buff->AddItemStats(entityLoot);
			}
			else
			{
				App->entityFactory->player->bagObjects.push_back(entityLoot);
			}
		}
	}
	else if (entityLoot->GetType() == LOOT_TYPE::CONSUMABLE)
	{
		if (entityLoot->GetObjectType() == OBJECT_TYPE::POTIONS)
			App->entityFactory->player->consumibles.push_back(entityLoot);

		else if (entityLoot->GetObjectType() == OBJECT_TYPE::GOLD)
		{
			App->entityFactory->player->gold += entityLoot->price;
			entityLoot->to_delete = true;
			return false;
		}

	}
	return true;
}
//LootEntityManager::LootEntityManager(iPoint pos) : j1Entity(LOOT, pos.x, pos.y, "LootItem")
//{
//	if (App->loot->GoldSystem)
//	{
//		gold = new Gold(pos.x, pos.y, App->loot->goldearned);
//		lootItems.push_back(gold);
//		lootList.push_back(gold);
//		selectedLootEntity = gold;
//		App->loot->GoldSystem = false;
//	}
//
//	//SelectedItem();
//}
//
//LootEntityManager::~LootEntityManager()
//{
//	delete gold;
//}
////void LootEntityManager::SelectedItem()
////{
////	if (App->loot->GoldSystem)
////	{
////		lootItems.push_back(gold);
////	}
////}
//
//bool LootEntityManager::Start()
//{
//	std::vector<LootEntity*>::iterator item = lootItems.begin();
//
//	for (; item != lootItems.end(); ++item)
//		(*item)->Start();
//
//	return true;
//}
//
//
//bool LootEntityManager::PreUpdate()
//{
//	return true;
//}
//
//bool LootEntityManager::Update(float dt)
//{
//
//	//SelectLoot();
//	
//
//	position = selectedLootEntity->position;
//	std::vector<LootEntity*>::iterator item = lootItems.begin();
//
//	for (; item != lootItems.end(); ++item)
//	{
//		(*item)->value = selectedLootEntity->value;
//	}
//	selectedLootEntity->Update(dt);
//	return true;
//}
//
//
//bool LootEntityManager::PostUpdate()
//{
//
//	position.x = App->loot->loot_posX;
//	/*std::vector
//	App->loot->loot_posX = App->loot->loot_pos.x;
//	App->loot->loot_pos = App->map->WorldToSubtileMap;*/
//
//	selectedLootEntity->PostUpdate();
//	return true;
//}
//
//
//bool LootEntityManager::CleanUp()
//{
//
//	std::vector<LootEntity*>::iterator item = lootItems.begin();
//
//	for (; item != lootItems.end(); ++item)
//	{
//		(*item)->CleanUp();
//	}
//	return true;
//}
//
//void LootEntityManager::SelectLoot()
//{
//
//	if (App->loot->GoldSystem)
//	{
//		gold = new Gold(position.x, position.y, App->loot->goldearned);
//		lootItems.push_back(gold);
//		lootList.push_back(gold);
//		selectedLootEntity = gold;
//		App->loot->GoldSystem = false;
//	}
//}