#include "j1Render.h"
#include "p2Log.h"
#include "j1Map.h"
#include "LootEntity.h"

LootEntity::LootEntity(int posX, int posY) : j1Entity(LOOT, posX, posY, "LootParent")
{
	//GetEnemySubtile(posX, posY);
}


LootEntity::~LootEntity()
{
}


bool Start()
{
	return true;
}


bool LootEntity::PreUpdate()
{
	return true;
}


bool LootEntity::Update(float dt)
{
	
	if (lootTex != nullptr)
	{
		App->render->Blit(lootTex, position.x, position.y, &rect_test);
	}
	return true;
}


bool LootEntity::PostUpdate()
{
	return true;
}


//bool LootEntity::CleanUp()
//{
//	return true;
//}

//TO FIX blit image,  for now   DRAW() isn't called
void LootEntity::Draw()
{
	if (lootTex != nullptr)
	{
		App->render->Blit(lootTex, position.x, position.y, &rect_test);
    }
}

void LootEntity::GetEnemySubtile(int x , int y)
{

	
	iPoint enemySubtile;
	if (enemy->life <= 0)
	{
		x =enemy->GetSubtilePos().x;
		y = enemy->GetSubtilePos().y;
	}

	enemySubtile = enemy->GetSubtilePos();
	x= enemySubtile.x;
	y = enemySubtile.y;
}
