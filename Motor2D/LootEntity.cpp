#include "j1Render.h"
#include "p2Log.h"
#include "j1Map.h"
#include "LootEntity.h"
#include "j1LootManager.h"
#include "p2Log.h"
#include <random>

LootEntity::LootEntity(LOOT_TYPE type, int posX, int posY) : j1Entity(LOOT, posX, posY, "LootParent"), loot_type(type)
{
	entityTex = App->tex->Load("textures/loot/loot_items.png");
	//ChooseEntity();

	//TO FIX if there is the more than one new  in the  switch constructor, crashes

}

LootEntity::~LootEntity()
{}


//bool LootEntity::PreUpdate()
//{
//	return true;
//}

bool LootEntity::Update(float dt)
{
	
	return true;
}

//bool LootEntity::PostUpdate()
//{
//	return true;
//}
//
//bool LootEntity::CleanUp()
//{
//	return true;
//}
std::string LootEntity::GetName()
{
	return name;
}

LOOT_TYPE LootEntity::GetType()
{

	return 	loot_type;

}

OBJECT_TYPE LootEntity::GetObjectType()
{
	return objectType;
}

//Quan arriba aqui per primera vegada ya ha fet 10 randoms WTF
int LootEntity::GetRandomValue(int min, int max)
{
	static std::default_random_engine generator;
	std::uniform_int_distribution<int> range(min, max);
	int ret = range(generator);

	LOG("RANDOM VALUE %i", ret);
	//LOG("value %i", ret_value);

	return ret;
}

void LootEntity::CreateBuff(BUFF_TYPE type, std::string character, std::string stat, ELEMENTAL_TYPE elementType, OBJECT_ROL rol, float value)
{
	stats.push_back(new Buff(type, character, stat, elementType, rol, value));
}

void LootEntity::Draw()
{
	//TO FIX only blits pivot
	if (entityTex != nullptr)
		App->render->Blit(entityTex, position.x, position.y, &loot_rect,1.0f);
	//LOG("painting loot");
}

EQUIPABLE_TYPE LootEntity::GetEquipable()
{
	return equipableType;
}