#include "j1EntityFactory.h"
#include "j1Render.h"
#include "p2Log.h"
#include "EnemyTest.h"
#include "Enemy.h"
#include "j1BuffManager.h"
#include "j1Scene.h"
#include "LootEntity.h"
#include "j1LootManager.h"
#include <algorithm>

j1EntityFactory::j1EntityFactory()
{
	name.assign("entities");
}

j1EntityFactory::~j1EntityFactory()
{
}

bool j1EntityFactory::Awake(pugi::xml_node & node)
{
	bool ret = true;

	pugi::xml_node entity;
	for (entity = node.child("entities").child("entity"); entity && ret; entity = entity.next_sibling("entity"))
	{
		if (node.attribute("type").as_string() == "static")
			entities.push_back(CreateEntity(ENTITY_TYPE::ENTITY_STATIC, node.attribute("positionX").as_float(), node.attribute("positionY").as_float(), node.attribute("name").as_string()));

		else if (node.attribute("type").as_string() == "dinamic")
			entities.push_back(CreateEntity(ENTITY_TYPE::ENTITY_DYNAMIC, node.attribute("positionX").as_float(), node.attribute("positionY").as_float(), node.attribute("name").as_string()));
	}

	std::vector<j1Entity*>::iterator item = entities.begin();
	for (; item != entities.end(); ++item)
	{
		(*item)->LoadEntitydata(node);
	}

	return ret;
}

bool j1EntityFactory::Start()
{
	std::vector<j1Entity*>::iterator item = entities.begin();
	for (; item != entities.end(); ++item)
	{
		(*item)->Start();
	}
	//load texture
	return true;
}

bool j1EntityFactory::PreUpdate()
{
	bool ret = true;

	// logic / collisions
	std::vector<j1Entity*>::iterator item = entities.begin();
	for (; item != entities.end(); ++item)
	{
		if ((*item) != nullptr)
		{
			ret = ((*item)->PreUpdate());
		}
	}

	return true;
}

bool j1EntityFactory::Update(float dt)
{
	bool ret = true;

	std::vector<j1Entity*>::iterator item = entities.begin();
	for (; item != entities.end();)
	{
		if ((*item) != nullptr) 
		{
			bool createLoot = false;
			iPoint enemypos;

			if (!(*item)->to_delete)
			{
				ret = (*item)->Update(dt);
				ret = ((*item)->Move(dt));
				// updates entity associated tile positions tile and subtile
				(*item)->UpdateTilePositions();
				//LOG("entity subtile: %i,%i", (*item)->GetSubtilePos().x, (*item)->GetSubtilePos().y);

				draw_entities.push_back(*item);

				++item;
			}
			else
			{
				//if entit is diffetent to player create loot
				if ((*item)->type != ENTITY_TYPE::PLAYER)
				{
					createLoot = true;
					enemypos = {GetEnemySubtile((*item)).x, GetEnemySubtile((*item)).y };
				}
				(*item)->CleanUp();
				delete(*item);
				(*item) = nullptr;
				item = entities.erase(item);
			}
			if (createLoot)
			{
				App->lootManager->CreateLoot(SetLootPos(enemypos.x, enemypos.y).x, SetLootPos(enemypos.x, enemypos.y).y, "no");
			}
		}
	}

	return ret;
}

bool j1EntityFactory::PostUpdate()
{
	
	std::vector<j1Entity*>::iterator item = entities.begin();
	for (; item != entities.end(); ++item)
	{
		(*item)->PostUpdate();
	}
	std::sort(draw_entities.begin(), draw_entities.end(), j1EntityFactory::SortByYPos);

	std::vector<j1Entity*>::iterator drawItem = draw_entities.begin();
	for (; drawItem != draw_entities.end(); ++drawItem)
	{
		(*drawItem)->Draw();
		if(App->scene->debug)
			Debug(*drawItem);
	}

	draw_entities.clear();

	return true;
}

bool j1EntityFactory::CleanUp()
{
	bool ret = true;

	std::vector<j1Entity*>::reverse_iterator entitiesItem = entities.rbegin();

	while (entitiesItem != entities.rend())
	{
		(*entitiesItem)->CleanUp();
		RELEASE(*entitiesItem);
		++entitiesItem;
	}
	entities.clear();

	RELEASE_ARRAY(entitiesDataMap);


	//unload texture

	return ret;
}

j1Entity* j1EntityFactory::CreateEntity(ENTITY_TYPE type, int positionX, int positionY, std::string name)
{
	j1Entity* ret = nullptr; 

	std::vector<j1Entity*>::iterator item = entities.begin();
	for (; item != entities.end(); ++item)
	{
		if (*item == nullptr)
			break;
	}
	switch (type)
	{
	case NO_TYPE:
		break;
	case ENTITY_STATIC:
		//ret = new
		break;
	case ENTITY_DYNAMIC:
		break;

	case ENEMY_TEST:
		/*ret = new EnemyTest(iPoint(positionX, positionY));
		ret->type = ENEMY_TEST;
		ret->name = name; 
		entities.push_back(ret);
		LOG("Created a entity");*/
		break;
	case LOOT:
		ret = App->lootManager->CreateLootType(positionX, positionY);
		if (ret != nullptr)
		{
			ret->type = LOOT;
			ret->name = name;
			LoadLootData((LootEntity*)ret, App->config);

			entities.push_back(ret);
		}
		LOG("From factory Loot Entity");
		break;
	default:
		break;
	}

	return ret;
}

Enemy * j1EntityFactory::CreateEnemy(EnemyType etype,iPoint pos, uint speed, uint tilesDetectionRange, uint attackRange, float attackSpeed)
{
	Enemy* ret = nullptr; 

	switch (etype)
	{
	case EnemyType::TEST:
		ret = new EnemyTest(pos, speed, tilesDetectionRange, attackRange, attackSpeed); 
		entities.push_back(ret);
		break; 

	case EnemyType::MELEE:
		break; 

	case EnemyType::DISTANCE:
		break; 

	case EnemyType::TRAP:
		break;
	
	default:
		break;
	}
	

	return ret;
}

void j1EntityFactory::Debug(j1Entity* ent)
{
	fPoint entityPivotPos = ent->GetPivotPos();
	//App->render->DrawCircle(entityPivotPos.x, entityPivotPos.y, 3, 0, 255, 0,255 ,true); //TODO: improve drawcircle render (scale,camera)
	SDL_Rect section = { entityPivotPos.x - 1, entityPivotPos.y - 1, 3,3 };
	App->render->DrawQuad(section, 0, 255, 0, 255, true, true);
	//LOG("position:%f,%f", ent->position.x, ent->position.y);
}

PlayerEntityManager* j1EntityFactory::CreatePlayer(iPoint position)
{
	player = new PlayerEntityManager(position);

	if (player != nullptr)
	{
		entities.push_back(player);
		return player;
	}
	
	LOG("Failed to create player system");
	return nullptr;
}



bool j1EntityFactory::SortByYPos(const j1Entity * entity1, const j1Entity * entity2)
{
	return entity1->pivot.y + entity1->position.y < entity2->pivot.y + entity2->position.y;
}

void j1EntityFactory::CreateEntitiesDataMap(int width, int height)
{
	subtileWidth = width;
	subtileHeight = height;

	if (entitiesDataMap != nullptr)
	{
		//RELEASE(entitiesDataMap);
		RELEASE_ARRAY(entitiesDataMap);
	}

	entitiesDataMap = new entityDataMap[subtileWidth * subtileHeight];
	memset(entitiesDataMap, NULL, subtileWidth*subtileHeight);

	LOG("");
}

std::vector<j1Entity*>* j1EntityFactory::GetSubtileEntityVectorAt(const iPoint pos) const
{
	if (CheckSubtileMapBoundaries(pos))
		return &entitiesDataMap[GetSubtileEntityIndexAt(pos)].entities;
	else
	{
		//LOG("data out of boundaries, ignoring");
		return nullptr;
	}
}

bool j1EntityFactory::isThisSubtileEmpty(const iPoint pos) const
{
	if (CheckSubtileMapBoundaries(pos))
		return !entitiesDataMap[GetSubtileEntityIndexAt(pos)].isEmpty();
	else
		return false;
}

int j1EntityFactory::GetSubtileEntityIndexAt(const iPoint pos) const
{
	return (pos.y * subtileWidth) + pos.x;
}

void j1EntityFactory::AssignEntityToSubtile(j1Entity* entity) const
{
	if (CheckSubtileMapBoundaries(entity->GetSubtilePos()))
		entitiesDataMap[GetSubtileEntityIndexAt(entity->GetSubtilePos())].entities.push_back(entity);
	else
		LOG("Trying to assign entity out of boundaries, ignoring");
}

bool j1EntityFactory::DeleteEntityFromSubtile(j1Entity* entity) const
{
	bool ret = false;

	int index = GetSubtileEntityIndexAt(entity->GetPreviousSubtilePos());

	std::vector<j1Entity*>::iterator entityIterator = entitiesDataMap[index].entities.begin();

	for (; entityIterator != entitiesDataMap[index].entities.end(); ++entityIterator)
	{
		if (*entityIterator == entity)
		{
			//LOG("found");
			entitiesDataMap[index].entities.erase(entityIterator);
			ret = true;
			break;
		}
	}

	return ret;
}

bool j1EntityFactory::isPlayerAdjacent(const iPoint & pos) const
{
	iPoint playerPos = player->GetSubtilePos(); 
	
	return (abs(playerPos.x - pos.x) <= 1 &&  abs(playerPos.y - pos.y) <=1 );
}

iPoint j1EntityFactory::TranslateToRelativePlayerPos(const iPoint & pos) const
{
	iPoint playerPos = player->GetSubtilePos(); 
	return iPoint((pos.x - playerPos.x) +1, (pos.y - playerPos.y) +1);
}

int j1EntityFactory::GetAdjacentIndex(const iPoint & pos) const
{
	return (pos.y * 3) + pos.x;
}

void j1EntityFactory::ReserveAdjacent(const iPoint& pos)
{
	if (isPlayerAdjacent(pos))
	{
		iPoint reserve = TranslateToRelativePlayerPos(pos);
		reservedAdjacentSubtiles[GetAdjacentIndex(reserve)] = true; 
		LOG("Reserved adjacent: %i x , %i y.", reserve.x, reserve.y);
	}
}

void j1EntityFactory::FreeAdjacent(const iPoint & pos)
{
	if (isPlayerAdjacent(pos))
	{
		iPoint reserve = TranslateToRelativePlayerPos(pos);
		reservedAdjacentSubtiles[GetAdjacentIndex(reserve)] = false;
		LOG("Released reserved adjacent: %i x , %i y.", reserve.x, reserve.y);
	}
}

bool j1EntityFactory::isThisSubtileReserved(const iPoint & pos) const
{
	if (isPlayerAdjacent(pos))
	{
		iPoint aux = TranslateToRelativePlayerPos(pos);
		return reservedAdjacentSubtiles[GetAdjacentIndex(aux)];
	}
	else return false; 
}

void j1EntityFactory::ReleaseAllReservedSubtiles()
{
	for (uint i = 0; i < 8; ++i)
	{
		reservedAdjacentSubtiles[i] = false; 
	}
	LOG("RELEASED ALL RESERVED SUBTILES");
}

bool j1EntityFactory::CheckSubtileMapBoundaries(const iPoint pos) const
{
	return (pos.x >= 0 && pos.x < subtileWidth &&
		pos.y >= 0 && pos.y < subtileHeight);
}

bool j1EntityFactory::LoadLootData(LootEntity * lootEntity, pugi::xml_node & config)
{
	int randID = 0;
	int id;
	switch (lootEntity->GetObjectType())
	{
	case OBJECT_TYPE::POTIONS:
		for (auto node : config.child("loot").child("potions").children("health_potion"))
		{
			lootEntity->CreateBuff(BUFF_TYPE::ADDITIVE, App->entityFactory->player, "\0", ELEMENTAL_TYPE::NORMAL_ELEMENT, ROL::DEFENCE_ROL, node.attribute("value").as_int(), lootEntity);
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
							lootEntity->CreateBuff(BUFF_TYPE::ADDITIVE, lootEntity->character, "inteligence", lootEntity->elemetalType, ROL::ATTACK_ROL, GetRandomValue(1, 5), lootEntity);
							lootEntity->CreateBuff(BUFF_TYPE::MULTIPLICATIVE, lootEntity->character, "inteligence", lootEntity->elemetalType, ROL::DEFENCE_ROL, GetRandomValue(10, 15)* 0.01, lootEntity);
						}
						else if (lootEntity->level == 2)
						{
							lootEntity->CreateBuff(BUFF_TYPE::ADDITIVE, lootEntity->character, "inteligence", lootEntity->elemetalType, ROL::ATTACK_ROL, GetRandomValue(6, 10), lootEntity);
							lootEntity->CreateBuff(BUFF_TYPE::MULTIPLICATIVE, lootEntity->character, "inteligence", lootEntity->elemetalType, ROL::DEFENCE_ROL, GetRandomValue(16, 20)* 0.01, lootEntity);
						}
						else
						{
							lootEntity->CreateBuff(BUFF_TYPE::ADDITIVE, lootEntity->character, "inteligence", lootEntity->elemetalType, ROL::ATTACK_ROL, GetRandomValue(11, 15), lootEntity);
							lootEntity->CreateBuff(BUFF_TYPE::MULTIPLICATIVE, lootEntity->character, "inteligence", lootEntity->elemetalType, ROL::DEFENCE_ROL, GetRandomValue(21, 25)* 0.01, lootEntity);
						}


					}

					else if (id == 2)
					{
						if (lootEntity->level == 1)
						{
							lootEntity->CreateBuff(BUFF_TYPE::MULTIPLICATIVE, lootEntity->character, "inteligence", lootEntity->elemetalType, ROL::ATTACK_ROL, GetRandomValue(10, 20) * 0.01, lootEntity);
							lootEntity->CreateBuff(BUFF_TYPE::MULTIPLICATIVE, lootEntity->character, "inteligence", ELEMENTAL_TYPE::ALL_ELEMENTS, ROL::DEFENCE_ROL, GetRandomValue(10, 15)* 0.01, lootEntity);
						}
						else if (lootEntity->level == 2)
						{
							lootEntity->CreateBuff(BUFF_TYPE::MULTIPLICATIVE, lootEntity->character, "inteligence", lootEntity->elemetalType, ROL::ATTACK_ROL, GetRandomValue(16, 30) * 0.01, lootEntity);
							lootEntity->CreateBuff(BUFF_TYPE::MULTIPLICATIVE, lootEntity->character, "inteligence", ELEMENTAL_TYPE::ALL_ELEMENTS, ROL::DEFENCE_ROL, GetRandomValue(16, 20)* 0.01, lootEntity);
						}
						else
						{
							lootEntity->CreateBuff(BUFF_TYPE::MULTIPLICATIVE, lootEntity->character, "inteligence", lootEntity->elemetalType, ROL::ATTACK_ROL, GetRandomValue(31, 45) * 0.01, lootEntity);
							lootEntity->CreateBuff(BUFF_TYPE::MULTIPLICATIVE, lootEntity->character, "inteligence", ELEMENTAL_TYPE::ALL_ELEMENTS, ROL::DEFENCE_ROL, GetRandomValue(21, 25)* 0.01, lootEntity);
						}

					}
					else
					{
						if (lootEntity->level == 1)
						{
							lootEntity->CreateBuff(BUFF_TYPE::ADDITIVE, lootEntity->character, "\0", lootEntity->elemetalType, ROL::ATTACK_ROL, GetRandomValue(1, 5), lootEntity);
							lootEntity->CreateBuff(BUFF_TYPE::MULTIPLICATIVE, lootEntity->character, "\0", ELEMENTAL_TYPE::ALL_ELEMENTS, ROL::DEFENCE_ROL, GetRandomValue(10, 15)* 0.01, lootEntity);
						}
						else if (lootEntity->level == 2)
						{
							lootEntity->CreateBuff(BUFF_TYPE::ADDITIVE, lootEntity->character, "\0", lootEntity->elemetalType, ROL::ATTACK_ROL, GetRandomValue(6, 10), lootEntity);
							lootEntity->CreateBuff(BUFF_TYPE::MULTIPLICATIVE, lootEntity->character, "\0", ELEMENTAL_TYPE::ALL_ELEMENTS, ROL::DEFENCE_ROL, GetRandomValue(16, 20)* 0.01, lootEntity);
						}
						else
						{
							lootEntity->CreateBuff(BUFF_TYPE::ADDITIVE, lootEntity->character, "\0", lootEntity->elemetalType, ROL::ATTACK_ROL, GetRandomValue(11, 15), lootEntity);
							lootEntity->CreateBuff(BUFF_TYPE::MULTIPLICATIVE, lootEntity->character, "\0", ELEMENTAL_TYPE::ALL_ELEMENTS, ROL::DEFENCE_ROL, GetRandomValue(21, 25)* 0.01, lootEntity);
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

int j1EntityFactory::GetRandomValue(int min, int max)
{
	static std::default_random_engine generator;
	std::uniform_int_distribution<int> range(min, max);
	int ret_value = range(generator);

	LOG("RANDOM VALUE %i", ret_value);
	//LOG("value %i", ret_value);

	return ret_value;
}

j1Entity * j1EntityFactory::CreateLootType(int x, int y)
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

LOOT_TYPE j1EntityFactory::WillDrop()
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

iPoint j1EntityFactory::GetEnemySubtile(j1Entity * enemy)
{
	return  enemy->GetSubtilePos();

}

iPoint j1EntityFactory::SetLootPos(int x, int y)
{
	return App->map->SubTileMapToWorld(x, y);
}
