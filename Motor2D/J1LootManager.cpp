#include "j1LootManager.h"
#include "j1Render.h"
#include "p2Log.h"
#include "j1Map.h"
#include "j1EntityFactory.h"
#include "j1BuffManager.h"
#include "j1Scene.h"
#include "UiItem.h"
#include "UiItem_Label.h"
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
	 LoadLootData(entityLoot, App->config);
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
			 App->entityFactory->player->str_coin = "x  " + std::to_string(App->entityFactory->player->gold);
			 App->scene->coins_label->ChangeTextureIdle(App->entityFactory->player->str_coin, NULL, NULL);
			 return false;
		 }

	 }
	 return true;
 }

 bool j1LootManager::LoadLootData(LootEntity* lootEntity, pugi::xml_node& config)
 {

	 int randID = 0;
	 int id;
	 switch (lootEntity->GetObjectType())
	 {
	 case OBJECT_TYPE::POTIONS:
		 for (auto node : config.child("loot").child("potions").children("health_potion"))
		 {
			 lootEntity->CreateBuff(BUFF_TYPE::ADDITIVE, nullptr, "\0", ELEMENTAL_TYPE::NORMAL_ELEMENT, OBJECT_ROL::DEFENCE_ROL, node.attribute("value").as_int(), lootEntity);
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

		 randID = GetRandomValue(1, 3);

		 switch (lootEntity->GetEquipable())
		 {
		 case EQUIPABLE_TYPE::SWORD:
			 lootEntity->character = App->entityFactory->player->GetMarche();
			 for (auto node : config.child("loot").child("equipable").child("sword").children("equipment"))
			 {
				 //weapon type
				 id = node.attribute("id").as_int();

				 if (id == randID)
				 {
					 switch (GetRandomValue(1, 12))
					 {
					 case 1:
						 lootEntity->elemetalType = ELEMENTAL_TYPE::FIRE_ELEMENT;
						 lootEntity->level = 1;
						 break;
					 case 2:
						 lootEntity->elemetalType = ELEMENTAL_TYPE::ICE_ELEMENT;
						 lootEntity->level = 1;
						 break;
					 case 3:
						 lootEntity->elemetalType = ELEMENTAL_TYPE::POISON_ELEMENT;
						 lootEntity->level = 1;
						 break;
					 case 4:
						 lootEntity->elemetalType = ELEMENTAL_TYPE::NORMAL_ELEMENT;
						 lootEntity->level = 1;
						 break;
					 case 5:
						 lootEntity->elemetalType = ELEMENTAL_TYPE::FIRE_ELEMENT;
						 lootEntity->level = 2;
						 break;
					 case 6:
						 lootEntity->elemetalType = ELEMENTAL_TYPE::ICE_ELEMENT;
						 lootEntity->level = 2;
						 break;
					 case 7:
						 lootEntity->elemetalType = ELEMENTAL_TYPE::POISON_ELEMENT;
						 lootEntity->level = 2;
						 break;
					 case 8:
						 lootEntity->elemetalType = ELEMENTAL_TYPE::NORMAL_ELEMENT;
						 lootEntity->level = 2;
						 break;
					 case 9:
						 lootEntity->elemetalType = ELEMENTAL_TYPE::FIRE_ELEMENT;
						 lootEntity->level = 3;
						 break;
					 case 10:
						 lootEntity->elemetalType = ELEMENTAL_TYPE::ICE_ELEMENT;
						 lootEntity->level = 3;
						 break;
					 case 11:
						 lootEntity->elemetalType = ELEMENTAL_TYPE::POISON_ELEMENT;
						 lootEntity->level = 3;
						 break;
					 case 12:
						 lootEntity->elemetalType = ELEMENTAL_TYPE::NORMAL_ELEMENT;
						 lootEntity->level = 3;
						 break;
					 default:
						 break;
					 }
					 if (id == 1)
					 {
						 if (lootEntity->level == 1)
						 {
							 lootEntity->CreateBuff(BUFF_TYPE::ADDITIVE, lootEntity->character, "inteligence", lootEntity->elemetalType, OBJECT_ROL::ATTACK_ROL, GetRandomValue(1, 5), lootEntity);
							 lootEntity->CreateBuff(BUFF_TYPE::MULTIPLICATIVE, lootEntity->character, "inteligence", lootEntity->elemetalType, OBJECT_ROL::DEFENCE_ROL, GetRandomValue(10, 15)* 0.01, lootEntity);
						 }
						 else if (lootEntity->level == 2)
						 {
							 lootEntity->CreateBuff(BUFF_TYPE::ADDITIVE, lootEntity->character, "inteligence", lootEntity->elemetalType, OBJECT_ROL::ATTACK_ROL, GetRandomValue(6, 10), lootEntity);
							 lootEntity->CreateBuff(BUFF_TYPE::MULTIPLICATIVE, lootEntity->character, "inteligence", lootEntity->elemetalType, OBJECT_ROL::DEFENCE_ROL, GetRandomValue(16, 20)* 0.01, lootEntity);
						 }
						 else
						 {
							 lootEntity->CreateBuff(BUFF_TYPE::ADDITIVE, lootEntity->character, "inteligence", lootEntity->elemetalType, OBJECT_ROL::ATTACK_ROL, GetRandomValue(11, 15), lootEntity);
							 lootEntity->CreateBuff(BUFF_TYPE::MULTIPLICATIVE, lootEntity->character, "inteligence", lootEntity->elemetalType, OBJECT_ROL::DEFENCE_ROL, GetRandomValue(21, 25)* 0.01, lootEntity);
						 }


					 }

					 else if (id == 2)
					 {
						 if (lootEntity->level == 1)
						 {
							 lootEntity->CreateBuff(BUFF_TYPE::MULTIPLICATIVE, lootEntity->character, "inteligence", lootEntity->elemetalType, OBJECT_ROL::ATTACK_ROL, GetRandomValue(10, 20) * 0.01, lootEntity);
							 lootEntity->CreateBuff(BUFF_TYPE::MULTIPLICATIVE, lootEntity->character, "inteligence", ELEMENTAL_TYPE::NORMAL_ELEMENT, OBJECT_ROL::DEFENCE_ROL, GetRandomValue(10, 15)* 0.01, lootEntity);
						 }
						 else if (lootEntity->level == 2)
						 {
							 lootEntity->CreateBuff(BUFF_TYPE::MULTIPLICATIVE, lootEntity->character, "inteligence", lootEntity->elemetalType, OBJECT_ROL::ATTACK_ROL, GetRandomValue(16, 30) * 0.01, lootEntity);
							 lootEntity->CreateBuff(BUFF_TYPE::MULTIPLICATIVE, lootEntity->character, "inteligence", ELEMENTAL_TYPE::NORMAL_ELEMENT, OBJECT_ROL::DEFENCE_ROL, GetRandomValue(16, 20)* 0.01, lootEntity);
						 }
						 else
						 {
							 lootEntity->CreateBuff(BUFF_TYPE::MULTIPLICATIVE, lootEntity->character, "inteligence", lootEntity->elemetalType, OBJECT_ROL::ATTACK_ROL, GetRandomValue(31, 45) * 0.01, lootEntity);
							 lootEntity->CreateBuff(BUFF_TYPE::MULTIPLICATIVE, lootEntity->character, "inteligence", ELEMENTAL_TYPE::NORMAL_ELEMENT, OBJECT_ROL::DEFENCE_ROL, GetRandomValue(21, 25)* 0.01, lootEntity);
						 }

					 }
					 else
					 {
						 if (lootEntity->level == 1)
						 {
							 lootEntity->CreateBuff(BUFF_TYPE::ADDITIVE, lootEntity->character, "\0", lootEntity->elemetalType, OBJECT_ROL::ATTACK_ROL, GetRandomValue(1, 5), lootEntity);
							 lootEntity->CreateBuff(BUFF_TYPE::MULTIPLICATIVE, lootEntity->character, "\0", ELEMENTAL_TYPE::NORMAL_ELEMENT, OBJECT_ROL::DEFENCE_ROL, GetRandomValue(10, 15)* 0.01, lootEntity);
						 }
						 else if (lootEntity->level == 2)
						 {
							 lootEntity->CreateBuff(BUFF_TYPE::ADDITIVE, lootEntity->character, "\0", lootEntity->elemetalType, OBJECT_ROL::ATTACK_ROL, GetRandomValue(6, 10), lootEntity);
							 lootEntity->CreateBuff(BUFF_TYPE::MULTIPLICATIVE, lootEntity->character, "\0", ELEMENTAL_TYPE::NORMAL_ELEMENT, OBJECT_ROL::DEFENCE_ROL, GetRandomValue(16, 20)* 0.01, lootEntity);
						 }
						 else
						 {
							 lootEntity->CreateBuff(BUFF_TYPE::ADDITIVE, lootEntity->character, "\0", lootEntity->elemetalType, OBJECT_ROL::ATTACK_ROL, GetRandomValue(11, 15), lootEntity);
							 lootEntity->CreateBuff(BUFF_TYPE::MULTIPLICATIVE, lootEntity->character, "\0", ELEMENTAL_TYPE::NORMAL_ELEMENT, OBJECT_ROL::DEFENCE_ROL, GetRandomValue(21, 25)* 0.01, lootEntity);
						 }
					 }
				 }
			 }
			 break;

			 /*	 case EQUIPABLE_TYPE::BOW:

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
				  break;*/
				  /*case OBJECT_TYPE::ARMOR_OBJECT:
				  break;
				  case OBJECT_TYPE::HEAD_OBJECT:
				  break;*/

				  /*case OBJECT_TYPE::NO_OBJECT:
				  break;*/

		 default:
			 break;
		 }
	 default:
		 break;
	 }
	 return true;
 }
