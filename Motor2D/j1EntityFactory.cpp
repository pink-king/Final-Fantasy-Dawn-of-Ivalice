#include "j1EntityFactory.h"
#include "j1Render.h"
#include "p2Log.h"
#include "EnemyTest.h"
#include "EnemyBomb.h"
#include "Enemy.h"
#include "j1BuffManager.h"
#include "j1Scene.h"
#include "LootEntity.h"
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
	assetsAtlasTex = App->tex->Load("maps/iso-64x64-building.png");

	enemyZombieTex = App->tex->Load("textures/enemies/enemyZombie.png");
	enemyBombTex = App->tex->Load("textures/enemies/enemyBomb.png");
	debugsubtileTex = App->tex->Load("maps/tile_32x32_2.png");

	/*constexpr char inits[] = __TIME__;
	const int default_seed = (inits[0] - '0') * 100000 + (inits[1] - '0') * 10000 +
		(inits[3] - '0') * 1000 + (inits[4] - '0') * 100 + (inits[6] - '0') * 10 + inits[7] - '0';*/

	//gen.seed(default_seed);

	gen.seed(rd()); //Standard mersenne_twister_engine seeded with rd()
	justGold = false;
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
				if ((*item)->type != ENTITY_TYPE::PLAYER && (*item)->type != ENTITY_TYPE::LOOT) //needs to be loot too, otherwise if player collects loot thereis teh cnahce to create loot again
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
				CreateLoot(SetLootPos(enemypos.x, enemypos.y).x, SetLootPos(enemypos.x, enemypos.y).y);
				justGold = true;
				CreateGold(SetLootPos(enemypos.x, enemypos.y).x, SetLootPos(enemypos.x, enemypos.y).y);
				

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
	App->tex->UnLoad(texture);
	App->tex->UnLoad(assetsAtlasTex);
	App->tex->UnLoad(enemyZombieTex);
	App->tex->UnLoad(enemyBombTex);
	App->tex->UnLoad(debugsubtileTex);

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
		/*ret = CreateLootType(positionX, positionY);
		if (ret != nullptr)
		{
			ret->type = LOOT;
			ret->name = name;
			LoadLootData((LootEntity*)ret, App->config);

			entities.push_back(ret);
		}*/
		LOG("From factory Loot Entity");
		break;
	default:
		break;
	}

	return ret;
}

Enemy * j1EntityFactory::CreateEnemy(EnemyType etype,iPoint pos, uint speed, uint tilesDetectionRange, uint attackRange, uint baseDamage, float attackSpeed)
{
	Enemy* ret = nullptr; 

	switch (etype)
	{
	case EnemyType::BOMB:
		ret = new EnemyBomb(pos, speed, tilesDetectionRange, attackRange, baseDamage);
		entities.push_back(ret);
		break;
	case EnemyType::TEST:
		ret = new EnemyTest(pos, speed, tilesDetectionRange, attackRange, baseDamage, attackSpeed); 
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

void j1EntityFactory::CreateEnemiesGroup(std::vector<EnemyType> enemyTypes, SDL_Rect zone, uint minNum, uint maxNum)
{
	j1Entity* ret = nullptr;
	uint numEnemies = CreateRandomBetween(minNum, maxNum);

	uint numBombs = 0;
	uint numTests = 0;
	uint cont = 0;

	uint bombProbs = 1;
	uint testProbs = 8;

	while (cont < numEnemies)
	{
		for (std::vector<EnemyType>::iterator typeIter = enemyTypes.begin(); typeIter != enemyTypes.end(); typeIter++)
		{
			switch (*typeIter)
			{
			case EnemyType::BOMB:
				if (CreateRandomBetween(1, 10) == bombProbs)
				{
					iPoint spawnPos = { zone.x + (int)CreateRandomBetween(0, zone.w), zone.y + (int)CreateRandomBetween(0,zone.h)};

					ret = CreateEnemy(EnemyType::BOMB, spawnPos, 70, 5, 1, 10, 1.5F);
					App->buff->CreateBuff(BUFF_TYPE::ADDITIVE, ELEMENTAL_TYPE::ALL_ELEMENTS, ROL::ATTACK_ROL, ret, "\0", CreateRandomBetween(0, 30));
					App->buff->CreateBuff(BUFF_TYPE::ADDITIVE, ELEMENTAL_TYPE::ALL_ELEMENTS, ROL::DEFENCE_ROL, ret, "\0", CreateRandomBetween(0, 10));
					numBombs++;
					cont++;
				}

				break;

			case EnemyType::TEST:
				if (CreateRandomBetween(1, 10) <= testProbs && cont < numEnemies)
				{
					iPoint spawnPos = { zone.x + (int)CreateRandomBetween(0, zone.w), zone.y + (int)CreateRandomBetween(0,zone.h) };
					ret = CreateEnemy(EnemyType::TEST, spawnPos, 70, 5, 1, 10, 1.5F);
					App->buff->CreateBuff(BUFF_TYPE::ADDITIVE, ELEMENTAL_TYPE::ALL_ELEMENTS, ROL::ATTACK_ROL, ret, "\0", CreateRandomBetween(0, 20));
					App->buff->CreateBuff(BUFF_TYPE::ADDITIVE, ELEMENTAL_TYPE::ALL_ELEMENTS, ROL::DEFENCE_ROL, ret, "\0", CreateRandomBetween(0, 50));
					numTests++;
					cont++;
				}
				break;

			}
		}
	}
	LOG("Created %i Enemies", numEnemies);
	LOG("Ceated %i TESTS", numTests);
	LOG("Ceated %i BOMBS", numBombs);


}

LootEntity* j1EntityFactory::CreateLoot(/*LOOT_TYPE lType,*/ int posX, int posY)
{
	LootEntity* ret = nullptr;
	 

	switch (WillDrop())
	{
	case LOOT_TYPE::EQUIPABLE:
		ret = new Equipable(posX, posY);
			LoadLootData(ret, App->config);
			entities.push_back(ret);
			break;

	case LOOT_TYPE::CONSUMABLE:
		ret = new Consumable(posX, posY);
		//ret->objectType = OBJECT_TYPE::GOLD;
		LoadLootData(ret, App->config);
		entities.push_back(ret);
		break;
	}
	return ret;
	
}

LootEntity* j1EntityFactory::CreateGold(int posX, int posY)
{
	LootEntity* ret = nullptr;
	if (GetRandomValue(1, 4) == 1)
	{
		ret = new Consumable(posX, posY);
		LoadLootData(ret, App->config);
		entities.push_back(ret);
	}
	return nullptr;
}

uint j1EntityFactory::CreateRandomBetween(uint min, uint max)
{
	std::uniform_real_distribution<float> dis(min, max);

	return (uint)dis(gen);
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

bool j1EntityFactory::isThisSubtileEnemyFree(const iPoint pos) const
{
	bool ret = false; 

	if (CheckSubtileMapBoundaries(pos))
	{
		ret = true; 
		std::vector<j1Entity*>::iterator entityIterator = entitiesDataMap[GetSubtileEntityIndexAt(pos)].entities.begin();
		for (; entityIterator != entitiesDataMap[GetSubtileEntityIndexAt(pos)].entities.end(); ++entityIterator)
		{
			if ((*entityIterator)->type == ENTITY_TYPE::ENEMY_TEST) // || other enemy types 
			{
				ret = false;
				break;
			}
		}
	}

	return ret;
}

int j1EntityFactory::GetSubtileEntityIndexAt(const iPoint pos) const
{
	return (pos.y * subtileWidth) + pos.x;
}

bool j1EntityFactory::areAllSubtilesReserved() const
{
	bool ret = true;
	for (uint i = 0; i < 8; ++i)
	{
		if (reservedAdjacentSubtiles[i] == false)
		{
			if (i != 4)
			{
				ret = false;
				break;
			}
		}
	}

	return ret;
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

void j1EntityFactory::CreateAsset(EnvironmentAssetsTypes type, iPoint worldPos, SDL_Rect atlasSpriteRect)
{
	j1Entity* assetEntity = nullptr;

	switch (type)
	{
	case EnvironmentAssetsTypes::NO_TYPE:
		break;
	case EnvironmentAssetsTypes::WALL:
		assetEntity = new j1Entity(worldPos, atlasSpriteRect);
		entities.push_back(assetEntity);
		break;
	case EnvironmentAssetsTypes::WALL1:
		break;
	case EnvironmentAssetsTypes::MAX:
		break;
	default:
		break;
	}
}
bool j1EntityFactory::LoadLootData(LootEntity * lootEntity, pugi::xml_node & config)
{
	int randID = 0;
	int id;
	int min, max;
	switch (lootEntity->GetObjectType())
	{
	case OBJECT_TYPE::POTIONS:
		for (auto node : config.child("loot").child("potions").children("health_potion"))
		{
			lootEntity->CreateBuff(BUFF_TYPE::ADDITIVE, App->entityFactory->player, "\0", ELEMENTAL_TYPE::NO_ELEMENT, ROL::DEFENCE_ROL, node.attribute("value").as_int(), lootEntity);
			LOG("looking for potions");
		}
		break;

	case OBJECT_TYPE::GOLD:

		min = config.child("loot").child("gold").attribute("min").as_int();
		max = config.child("loot").child("gold").attribute("max").as_int();
		lootEntity->price = GetRandomValue(min, max);
		LOG("gold amount %i", lootEntity->price);
		break;

	case OBJECT_TYPE::WEAPON_OBJECT:

		randID = GetRandomValue(1, 3);
		
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
			lootEntity->elemetalType = ELEMENTAL_TYPE::NO_ELEMENT;
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
			lootEntity->elemetalType = ELEMENTAL_TYPE::NO_ELEMENT;
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
			lootEntity->elemetalType = ELEMENTAL_TYPE::ALL_ELEMENTS;
			lootEntity->level = 3;
			break;
		default:
			break;
		}

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

					if (id == 1)
					{
						if (lootEntity->level == 1)
						{
							lootEntity->CreateBuff(BUFF_TYPE::ADDITIVE, lootEntity->character, "inteligence", lootEntity->elemetalType, ROL::ATTACK_ROL, GetRandomValue(20, 25), lootEntity);
							lootEntity->CreateBuff(BUFF_TYPE::MULTIPLICATIVE, lootEntity->character, "inteligence", ELEMENTAL_TYPE::NO_ELEMENT, ROL::COOLDOWN, GetRandomValue(75, 100)* 0.01, lootEntity);
						}
						else if (lootEntity->level == 2)
						{
							lootEntity->CreateBuff(BUFF_TYPE::ADDITIVE, lootEntity->character, "inteligence", lootEntity->elemetalType, ROL::ATTACK_ROL, GetRandomValue(26, 30), lootEntity);
							lootEntity->CreateBuff(BUFF_TYPE::MULTIPLICATIVE, lootEntity->character, "inteligence", ELEMENTAL_TYPE::NO_ELEMENT, ROL::COOLDOWN, GetRandomValue(50, 74)* 0.01, lootEntity);
						}
						else
						{
							lootEntity->CreateBuff(BUFF_TYPE::ADDITIVE, lootEntity->character, "inteligence", lootEntity->elemetalType, ROL::ATTACK_ROL, GetRandomValue(31, 35), lootEntity);
							lootEntity->CreateBuff(BUFF_TYPE::MULTIPLICATIVE, lootEntity->character, "inteligence", ELEMENTAL_TYPE::NO_ELEMENT, ROL::COOLDOWN, GetRandomValue(25, 49)* 0.01, lootEntity);
						}


					}

					else if (id == 2)
					{
						if (lootEntity->level == 1)
						{
							lootEntity->CreateBuff(BUFF_TYPE::MULTIPLICATIVE, lootEntity->character, "inteligence", lootEntity->elemetalType, ROL::ATTACK_ROL, GetRandomValue(10, 20) * 0.01, lootEntity);
							lootEntity->CreateBuff(BUFF_TYPE::ADDITIVE, lootEntity->character, "inteligence", lootEntity->elemetalType, ROL::DEFENCE_ROL, GetRandomValue(2, 8), lootEntity);
						}
						else if (lootEntity->level == 2)
						{
							lootEntity->CreateBuff(BUFF_TYPE::MULTIPLICATIVE, lootEntity->character, "inteligence", lootEntity->elemetalType, ROL::ATTACK_ROL, GetRandomValue(16, 30) * 0.01, lootEntity);
							lootEntity->CreateBuff(BUFF_TYPE::ADDITIVE, lootEntity->character, "inteligence", lootEntity->elemetalType, ROL::DEFENCE_ROL, GetRandomValue(9, 14), lootEntity);
						}
						else
						{
							lootEntity->CreateBuff(BUFF_TYPE::MULTIPLICATIVE, lootEntity->character, "inteligence", lootEntity->elemetalType, ROL::ATTACK_ROL, GetRandomValue(31, 45) * 0.01, lootEntity);
							lootEntity->CreateBuff(BUFF_TYPE::ADDITIVE, lootEntity->character, "inteligence", lootEntity->elemetalType, ROL::DEFENCE_ROL, GetRandomValue(15, 22), lootEntity);
						}

					}
					else
					{
						if (lootEntity->level == 1)
						{
							lootEntity->CreateBuff(BUFF_TYPE::ADDITIVE, lootEntity->character, "\0", lootEntity->elemetalType, ROL::ATTACK_ROL, GetRandomValue(1, 5), lootEntity);
							lootEntity->CreateBuff(BUFF_TYPE::MULTIPLICATIVE, lootEntity->character, "\0", lootEntity->elemetalType, ROL::DEFENCE_ROL, GetRandomValue(10, 15)* 0.01, lootEntity);
							lootEntity->CreateBuff(BUFF_TYPE::MULTIPLICATIVE, lootEntity->character, "inteligence", ELEMENTAL_TYPE::NO_ELEMENT, ROL::COOLDOWN, GetRandomValue(75, 100)* 0.01, lootEntity);
						}
						else if (lootEntity->level == 2)
						{
							lootEntity->CreateBuff(BUFF_TYPE::ADDITIVE, lootEntity->character, "\0", lootEntity->elemetalType, ROL::ATTACK_ROL, GetRandomValue(6, 10), lootEntity);
							lootEntity->CreateBuff(BUFF_TYPE::MULTIPLICATIVE, lootEntity->character, "\0", lootEntity->elemetalType, ROL::DEFENCE_ROL, GetRandomValue(16, 20)* 0.01, lootEntity);
							lootEntity->CreateBuff(BUFF_TYPE::MULTIPLICATIVE, lootEntity->character, "inteligence", ELEMENTAL_TYPE::NO_ELEMENT, ROL::COOLDOWN, GetRandomValue(50, 74)* 0.01, lootEntity);
						}
						else
						{
							lootEntity->CreateBuff(BUFF_TYPE::ADDITIVE, lootEntity->character, "\0", lootEntity->elemetalType, ROL::ATTACK_ROL, GetRandomValue(11, 15), lootEntity);
							lootEntity->CreateBuff(BUFF_TYPE::MULTIPLICATIVE, lootEntity->character, "\0", lootEntity->elemetalType, ROL::DEFENCE_ROL, GetRandomValue(21, 25)* 0.01, lootEntity);
							lootEntity->CreateBuff(BUFF_TYPE::MULTIPLICATIVE, lootEntity->character, "inteligence", ELEMENTAL_TYPE::NO_ELEMENT, ROL::COOLDOWN, GetRandomValue(25, 49)* 0.01, lootEntity);
						}
					}
				}
			}
			break;
		case EQUIPABLE_TYPE::BOW:
			lootEntity->character = App->entityFactory->player->GetShara();
			for (auto node : config.child("loot").child("equipable").child("bow").children("equipment"))
			{
				//weapon type
				id = node.attribute("id").as_int();

				if (id == randID)
				{
					if (id == 1)
					{
						if (lootEntity->level == 1)
						{
							lootEntity->CreateBuff(BUFF_TYPE::ADDITIVE, lootEntity->character, "inteligence", lootEntity->elemetalType, ROL::ATTACK_ROL, GetRandomValue(10, 15), lootEntity);
							lootEntity->CreateBuff(BUFF_TYPE::MULTIPLICATIVE, lootEntity->character, "inteligence", ELEMENTAL_TYPE::NO_ELEMENT, ROL::COOLDOWN, GetRandomValue(75, 100)* 0.01, lootEntity);
						}
						else if (lootEntity->level == 2)
						{
							lootEntity->CreateBuff(BUFF_TYPE::ADDITIVE, lootEntity->character, "inteligence", lootEntity->elemetalType, ROL::ATTACK_ROL, GetRandomValue(16, 20), lootEntity);
							lootEntity->CreateBuff(BUFF_TYPE::MULTIPLICATIVE, lootEntity->character, "inteligence", ELEMENTAL_TYPE::NO_ELEMENT, ROL::COOLDOWN, GetRandomValue(50, 74)* 0.01, lootEntity);
						}
						else
						{
							lootEntity->CreateBuff(BUFF_TYPE::ADDITIVE, lootEntity->character, "inteligence", lootEntity->elemetalType, ROL::ATTACK_ROL, GetRandomValue(21, 25), lootEntity);
							lootEntity->CreateBuff(BUFF_TYPE::MULTIPLICATIVE, lootEntity->character, "inteligence", ELEMENTAL_TYPE::NO_ELEMENT, ROL::COOLDOWN, GetRandomValue(50, 49)* 0.01, lootEntity);
						}


					}

					else if (id == 2)
					{
						if (lootEntity->level == 1)
						{
							lootEntity->CreateBuff(BUFF_TYPE::MULTIPLICATIVE, lootEntity->character, "inteligence", lootEntity->elemetalType, ROL::ATTACK_ROL, GetRandomValue(10, 20) * 0.01, lootEntity);
							lootEntity->CreateBuff(BUFF_TYPE::MULTIPLICATIVE, lootEntity->character, "inteligence", ELEMENTAL_TYPE::NO_ELEMENT, ROL::VELOCITY, GetRandomValue(2, 8)*0.01, lootEntity);
						}
						else if (lootEntity->level == 2)
						{
							lootEntity->CreateBuff(BUFF_TYPE::MULTIPLICATIVE, lootEntity->character, "inteligence", lootEntity->elemetalType, ROL::ATTACK_ROL, GetRandomValue(16, 30) * 0.01, lootEntity);
							lootEntity->CreateBuff(BUFF_TYPE::MULTIPLICATIVE, lootEntity->character, "inteligence", ELEMENTAL_TYPE::NO_ELEMENT, ROL::VELOCITY, GetRandomValue(9, 14)*0.01, lootEntity);
						}
						else
						{
							lootEntity->CreateBuff(BUFF_TYPE::MULTIPLICATIVE, lootEntity->character, "inteligence", lootEntity->elemetalType, ROL::ATTACK_ROL, GetRandomValue(31, 45) * 0.01, lootEntity);
							lootEntity->CreateBuff(BUFF_TYPE::MULTIPLICATIVE, lootEntity->character, "inteligence", ELEMENTAL_TYPE::NO_ELEMENT, ROL::VELOCITY, GetRandomValue(15, 22)*0.01, lootEntity);
						}

					}
					else
					{
						if (lootEntity->level == 1)
						{
							lootEntity->CreateBuff(BUFF_TYPE::ADDITIVE, lootEntity->character, "inteligence", lootEntity->elemetalType, ROL::ATTACK_ROL, GetRandomValue(10, 15), lootEntity);
							lootEntity->CreateBuff(BUFF_TYPE::MULTIPLICATIVE, lootEntity->character, "\0", ELEMENTAL_TYPE::NO_ELEMENT, ROL::COOLDOWN, GetRandomValue(75, 100)* 0.01, lootEntity);
							lootEntity->CreateBuff(BUFF_TYPE::MULTIPLICATIVE, lootEntity->character, "inteligence", ELEMENTAL_TYPE::NO_ELEMENT, ROL::VELOCITY, GetRandomValue(0, 6)*0.01, lootEntity);
						}
						else if (lootEntity->level == 2)
						{
							lootEntity->CreateBuff(BUFF_TYPE::ADDITIVE, lootEntity->character, "inteligence", lootEntity->elemetalType, ROL::ATTACK_ROL, GetRandomValue(16, 20), lootEntity);
							lootEntity->CreateBuff(BUFF_TYPE::MULTIPLICATIVE, lootEntity->character, "\0", ELEMENTAL_TYPE::NO_ELEMENT, ROL::COOLDOWN, GetRandomValue(50, 74)* 0.01, lootEntity);
							lootEntity->CreateBuff(BUFF_TYPE::MULTIPLICATIVE, lootEntity->character, "inteligence", ELEMENTAL_TYPE::NO_ELEMENT, ROL::VELOCITY, GetRandomValue(7, 12)*0.01, lootEntity);
						}
						else
						{
							lootEntity->CreateBuff(BUFF_TYPE::ADDITIVE, lootEntity->character, "inteligence", lootEntity->elemetalType, ROL::ATTACK_ROL, GetRandomValue(21, 25), lootEntity);
							lootEntity->CreateBuff(BUFF_TYPE::MULTIPLICATIVE, lootEntity->character, "\0", ELEMENTAL_TYPE::NO_ELEMENT, ROL::COOLDOWN, GetRandomValue(25, 49)* 0.01, lootEntity);
							lootEntity->CreateBuff(BUFF_TYPE::MULTIPLICATIVE, lootEntity->character, "inteligence", ELEMENTAL_TYPE::NO_ELEMENT, ROL::VELOCITY, GetRandomValue(13, 18)*0.01, lootEntity);
						}
					}
				}
			}
			break;

		case EQUIPABLE_TYPE::ROD:
			lootEntity->character = App->entityFactory->player->GetRitz();
			for (auto node : config.child("loot").child("equipable").child("rod").children("equipment"))
			{
				//weapon type
				id = node.attribute("id").as_int();

				if (id == randID)
				{
					if (id == 1)
					{
						if (lootEntity->level == 1)
						{
							lootEntity->CreateBuff(BUFF_TYPE::ADDITIVE, lootEntity->character, "inteligence", lootEntity->elemetalType, ROL::ATTACK_ROL, GetRandomValue(10, 20), lootEntity);
						}
						else if (lootEntity->level == 2)
						{
							lootEntity->CreateBuff(BUFF_TYPE::ADDITIVE, lootEntity->character, "inteligence", lootEntity->elemetalType, ROL::ATTACK_ROL, GetRandomValue(21, 30), lootEntity);
						}
						else
						{
							lootEntity->CreateBuff(BUFF_TYPE::ADDITIVE, lootEntity->character, "inteligence", lootEntity->elemetalType, ROL::ATTACK_ROL, GetRandomValue(31, 40), lootEntity);
						}
					}

					else if (id == 2)
					{
						if (lootEntity->level == 1)
						{
							lootEntity->CreateBuff(BUFF_TYPE::ADDITIVE, lootEntity->character, "inteligence", lootEntity->elemetalType, ROL::ATTACK_ROL, GetRandomValue(5, 10), lootEntity);
							lootEntity->CreateBuff(BUFF_TYPE::MULTIPLICATIVE, lootEntity->character, "inteligence", ELEMENTAL_TYPE::NO_ELEMENT, ROL::COOLDOWN, GetRandomValue(75, 100) * 0.01, lootEntity);
						}
						else if (lootEntity->level == 2)
						{
							lootEntity->CreateBuff(BUFF_TYPE::ADDITIVE, lootEntity->character, "inteligence", lootEntity->elemetalType, ROL::ATTACK_ROL, GetRandomValue(7, 17), lootEntity);
							lootEntity->CreateBuff(BUFF_TYPE::MULTIPLICATIVE, lootEntity->character, "inteligence", ELEMENTAL_TYPE::NO_ELEMENT, ROL::COOLDOWN, GetRandomValue(50, 74) * 0.01, lootEntity);
						}
						else
						{
							lootEntity->CreateBuff(BUFF_TYPE::ADDITIVE, lootEntity->character, "inteligence", lootEntity->elemetalType, ROL::ATTACK_ROL, GetRandomValue(16, 26), lootEntity);
							lootEntity->CreateBuff(BUFF_TYPE::MULTIPLICATIVE, lootEntity->character, "inteligence", ELEMENTAL_TYPE::NO_ELEMENT, ROL::VELOCITY, GetRandomValue(25, 49) * 0.01, lootEntity);
						}

					}
					else
					{
						if (lootEntity->level == 1)
						{
							lootEntity->CreateBuff(BUFF_TYPE::ADDITIVE, lootEntity->character, "inteligence", lootEntity->elemetalType, ROL::ATTACK_ROL, GetRandomValue(10, 20), lootEntity);
							lootEntity->CreateBuff(BUFF_TYPE::MULTIPLICATIVE, lootEntity->character, "inteligence", ELEMENTAL_TYPE::NO_ELEMENT, ROL::COOLDOWN, GetRandomValue(65, 85) * 0.01, lootEntity);
						}
						else if (lootEntity->level == 2)
						{
							lootEntity->CreateBuff(BUFF_TYPE::ADDITIVE, lootEntity->character, "inteligence", lootEntity->elemetalType, ROL::ATTACK_ROL, GetRandomValue(21, 30), lootEntity);
							lootEntity->CreateBuff(BUFF_TYPE::MULTIPLICATIVE, lootEntity->character, "inteligence", ELEMENTAL_TYPE::NO_ELEMENT, ROL::COOLDOWN, GetRandomValue(65, 85) * 0.01, lootEntity);
						}
						else
						{
							lootEntity->CreateBuff(BUFF_TYPE::ADDITIVE, lootEntity->character, "inteligence", lootEntity->elemetalType, ROL::ATTACK_ROL, GetRandomValue(31, 40), lootEntity);
							lootEntity->CreateBuff(BUFF_TYPE::MULTIPLICATIVE, lootEntity->character, "inteligence", ELEMENTAL_TYPE::NO_ELEMENT, ROL::COOLDOWN, GetRandomValue(65, 85) * 0.01, lootEntity);
						}
					}
				}
			}
			break;
		default:
			break;
		}
		case OBJECT_TYPE::ARMOR_OBJECT:
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

//j1Entity * j1EntityFactory::CreateLootType(int x, int y)
//{
//	j1Entity * ret = nullptr;
//
//	switch (WillDrop())
//	{
//	case LOOT_TYPE::CONSUMABLE:
//
//		ret = new Consumable(x, y);
//		break;
//
//	case LOOT_TYPE::EQUIPABLE:
//		ret = new Equipable(x, y);
//		break;
//
//	default:
//		break;
//	}
//
//	return ret;
//}

LOOT_TYPE j1EntityFactory::WillDrop()
{
	int randvalue = GetRandomValue(1, 100);

	if (randvalue <= lootChance)
	{
		if (randvalue <= 5)
			return  LOOT_TYPE::CONSUMABLE;

		else
			return  LOOT_TYPE::EQUIPABLE;


	}

	else return LOOT_TYPE::NO_LOOT;
	
}

iPoint j1EntityFactory::GetEnemySubtile(j1Entity * enemy)
{
	return  enemy->GetSubtilePos();

}

iPoint j1EntityFactory::SetLootPos(int x, int y)
{
	return App->map->SubTileMapToWorld(x, y);
}
