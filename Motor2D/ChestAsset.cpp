#include "ChestAsset.h"
#include "j1Render.h"
#include "j1EntityFactory.h"
#include "j1PathFinding.h"
#include "ChestTrigger.h"

ChestAsset::ChestAsset(const iPoint & pos, bool isBossChest) : isBossChest(isBossChest)
{
	// Rect Pos.y -10 to align to center of the tile 
	idle.PushBack({ 382, -10, 64, 64 });
	opening.PushBack({ 446, -10, 64, 64 });
	opening.PushBack({ 510, -10, 64, 64 });
	opening.PushBack({ 574, -10, 64, 64 });
	opening.speed = 10.F;
	opening.loop = false;

	// Ready to add a "Boss Chest" animation 

	currentAnimation = &idle;

	position.x = pos.x;
	position.y = pos.y;

	SetPivot(32, 55);
	size.create(64, 64);
}

ChestAsset::~ChestAsset()
{
}

bool ChestAsset::PreUpdate()
{
	if (!isAssigned)	// I have to do this the pretty way: booleans! :D
	{
		App->pathfinding->ActivateTile(App->map->WorldToMap(GetPivotPos().x, GetPivotPos().y));

		ChestTrigger* myTrigger = dynamic_cast<ChestTrigger*>(App->entityFactory->CreateTrigger(TRIGGER_TYPE::CHEST, GetPivotPos().x, GetPivotPos().y));
		if (myTrigger != nullptr)
			myTrigger->AssignOwner(this);

		isAssigned = true;
	}

	if (isOpened && currentAnimation->Finished())
	{
		SpawnLoot();
		isOpened = false;
	}

	return true;
}

void ChestAsset::Draw()
{
	if (entityTex != nullptr) // if we have any specific linked texture
	{
		App->render->Blit(entityTex, position.x, position.y, &currentAnimation->GetCurrentFrame(), 1.0F, flip);
	}
	else
		App->render->Blit(App->entityFactory->assetsAtlasTex, position.x, position.y, &currentAnimation->GetCurrentFrame());
}

void ChestAsset::OpenChest()
{
	isOpened = true;
	currentAnimation = &opening;
	//SpawnLoot();
	// TODO: Add SFX
}

void ChestAsset::SpawnLoot()
{
	if (isBossChest)
	{
		App->entityFactory->RepeatAmountofConsumables(15, GetPivotPos(), OBJECT_TYPE::GOLD);
		App->entityFactory->RandomAmountofLoot(LOOT_TYPE::EQUIPABLE, 3, GetPivotPos());
	}
	else
	{
		App->entityFactory->RepeatAmountofConsumables(7, GetPivotPos(), OBJECT_TYPE::GOLD);
		App->entityFactory->RandomAmountofLoot(LOOT_TYPE::EQUIPABLE, 1, GetPivotPos());
	}
}
