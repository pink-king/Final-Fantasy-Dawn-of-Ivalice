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

	//LOG("lootpos %i x %i y", lootPos);

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
    LoadLootData(entityLoot,App->config);
	

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

bool j1LootManager::LoadLootData(LootEntity* lootEntity, pugi::xml_node& config)
{
	//pugi::xml_document config_file;
	//config = App->config;
	/*pugi::xml_node config;
	pugi::xml_parse_result result = config_file.load_file("config.xml");
	if (result == NULL)
		LOG("Could not load map xml file config.xml. pugi error: %s", result.description());
	else
		config = config_file.child("config").child("loot");*/
	
	OBJECT_TYPE object = lootEntity->GetObjectType();
	int randID = 0;
	int id;
	
	switch (object)
	{
	case OBJECT_TYPE::POTIONS:
		for (auto node : config.child("loot").child("potions").children("health_potion"))
		{
			lootEntity->heal = node.attribute("value").as_int();
			LOG("looking for potions");
		}
		break;

	case OBJECT_TYPE::GOLD:
		//TO FIX: in a single collect item iteration this case is siwtched twice instead of a single time
		randID = GetRandomValue(1, 3);
		
		for (auto node : config.child("loot").child("Money").children("gold"))
		{
			id = node.attribute("id").as_int();
			
			if (id = randID)
			{
				lootEntity->price = node.attribute("value").as_int();
				//gold_id[i] = node.attribute("id").as_int();
				LOG("gold searched for");
				break;
			}
		}
		break;

	case OBJECT_TYPE::WEAPON_OBJECT:
		
		randID = GetRandomValue(1, 9);
		EQUIPABLE_TYPE equip = lootEntity->GetEquipable();
		Marche* marche;
		

		switch (equip)
		{
			case EQUIPABLE_TYPE::SWORD:
			
				


					randID = GetRandomValue(1, 9);
					for (auto node : config.child("loot").child("equipable").child("sword").children("equipment"))
					{
						id = node.attribute("id").as_int();

						if (id == randID)
						{
							lootEntity->itemLevel = node.attribute("level").as_int();

							if (id <= 3)
								lootEntity->dmg = node.attribute("dmg").as_float();

							else if (id > 3 && id <= 6)
								lootEntity->burn = node.attribute("burn").as_float();

							else
							{
								lootEntity->dmg = node.attribute("dmg").as_float();
								lootEntity->burn = node.attribute("burn").as_float();
							}
							LOG("looking for sword");
							
							
							
						}
					}
					LOG("hey");
			
			break;
			
		case EQUIPABLE_TYPE::BOW:
			
			for (auto node : config.child("loot").child("equipable").child("bow").children("equipment"))
			{
				id = node.attribute("id").as_int();

				if (id == randID) {
					lootEntity->itemLevel = node.attribute("level").as_int();

					if (id <= 3)
						lootEntity->attck_spd = node.attribute("attck_spd").as_float();

					else if (id > 3 && id <= 6)
						lootEntity->slow = node.attribute("slow").as_float();

					else
					{
						lootEntity->attck_spd = node.attribute("attck_spd").as_float();
						lootEntity->slow = node.attribute("slow").as_float();
					}
					LOG("looking for bow");
					break;
				}
			}
			break;

		case EQUIPABLE_TYPE::ROD:
			
			for (auto node : config.child("loot").child("equipable").child("rod").children("equipment"))
			{
				id = node.attribute("id").as_int();
				if (id == randID) {
					lootEntity->itemLevel = node.attribute("level").as_int();
					
					if (id <= 3)
					{
						lootEntity->chanceTo = node.attribute("chance").as_float();
						lootEntity->heal = node.attribute("heal").as_float();
					}

					else if (id > 3 && id <= 6)
					lootEntity->paralize = node.attribute("paralyze").as_float();

					else
					{
						lootEntity->paralize = node.attribute("paralyze").as_float();
						lootEntity->chanceTo = node.attribute("chance").as_float();
						lootEntity->heal = node.attribute("heal").as_float();
					}
					LOG("looking for rod");
					break;
				}
			}
			break;

		default:
			break;
		}
		break;
	/*case OBJECT_TYPE::ARMOR_OBJECT:
		break;
	case OBJECT_TYPE::HEAD_OBJECT:
		break;*/

	/*case OBJECT_TYPE::NO_OBJECT:
		break;*/
	
	}
return true;
}






