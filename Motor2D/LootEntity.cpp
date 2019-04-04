#include "j1Render.h"
#include "p2Log.h"
#include "j1Map.h"
#include "LootEntity.h"

LootEntity::LootEntity(int posX, int posY) : j1Entity(LOOT, posX, posY, "LootParent")
{
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

void LootEntity::Draw()
{
	if (lootTex != nullptr)
	{
		App->render->Blit(lootTex, position.x, position.y);
    }
}

void LootEntity::GetEnemySubtile(int x , int y)
{
	if (enemy_dead)
	{
		
	}
}
