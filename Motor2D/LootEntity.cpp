#include "j1Render.h"
#include "p2Log.h"
#include "j1Map.h"
#include "LootEntity.h"
#include "j1LootManager.h"
#include "p2Log.h"
#include <random>

LootEntity::LootEntity(int posX, int posY) : j1Entity(LOOT, posX, posY, "LootParent")
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
	//Draw();
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

void LootEntity::Draw()
{
	//TO FIX only blits pivot
	if (entityTex != nullptr)
		App->render->Blit(entityTex, position.x, position.y, &loot_rect);
	//LOG("painting loot");

	
}
//LootEntity::LootEntity(int posX, int posY) : j1Entity(LOOT, posX, posY, "LootParent")
//{
//	SetLootSubtile(posX, posY);
//	LOG("LootEntity Created");
//}
//
//
//LootEntity::~LootEntity()
//{
//}
//
//
//bool Start()
//{
//	return true;
//}
//
//
//bool LootEntity::PreUpdate()
//{
//	return true;
//}
//
//
//bool LootEntity::Update(float dt)
//{
//	player->GetSubtilePos();
//	if (lootTex != nullptr)
//	{
//		App->render->Blit(lootTex, position.x, position.y, &rect_test);
//	}
//	return true;
//}
//
//
//bool LootEntity::PostUpdate()
//{
//	return true;
//}
//
//
////bool LootEntity::CleanUp()
////{
////	return true;
////}
//
////TO FIX blit image,  for now   DRAW() isn't called
//void LootEntity::Draw()
//{
//	if (lootTex != nullptr)
//	{
//		App->render->Blit(lootTex, position.x, position.y, &rect_test);
//    }
//}
//
//void LootEntity::SetLootSubtile(int x , int y)
//{
//
//	
//	iPoint enemyPosition;
//	enemyPosition = App->map->SubTileMapToWorld(App->lootManager->loot_pos.x, App->lootManager->loot_pos.y);
//	LOG("LootSUbtilePos %i X  %i Y", enemyPosition.x, enemyPosition.y);
//	App->lootManager->loot_posX = enemyPosition.x;
//	App->lootManager->loot_posY = enemyPosition.y;
//	x = enemyPosition.x;
//	y = enemyPosition.y;
//}
