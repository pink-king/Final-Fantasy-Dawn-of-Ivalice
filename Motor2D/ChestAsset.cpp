#include "ChestAsset.h"
#include "j1Render.h"
#include "j1EntityFactory.h"
#include "j1PathFinding.h"
#include "ChestTrigger.h"

ChestAsset::ChestAsset(const iPoint & pos, bool isOpened, ChestType type) : isOpened(isOpened), type(type), j1Entity(ENTITY_TYPE::CHEST, pos.x,pos.y,"chest")
{
	switch (type)
	{
	case ChestType::BOSS:

		idle.PushBack({ 256 + 384, 192 - 10, 64, 64 });
		opening.PushBack({ 320 + 384, 192 - 10, 64, 64 });
		opening.PushBack({ 384 + 384, 192 - 10, 64, 64 });
		opening.PushBack({ 448 + 384, 192 - 10, 64, 64 });
		opening.speed = 10.F;
		opening.loop = false;
		break; 

	case ChestType::NORMAL:
		// Rect.y - 4
		idle.PushBack({ 574, 380, 64, 64 });
		opening.PushBack({ 638, 380, 64, 64 });
		opening.PushBack({ 638, 380, 64, 64 });
		opening.speed = 10.F;
		opening.loop = false;
		break;

	case ChestType::SILVER:
		idle.PushBack({ 256 + 384, 256 - 10, 64, 64 });
		opening.PushBack({ 320 + 384, 256 - 10, 64, 64 });
		opening.PushBack({ 384 + 384, 256 - 10, 64, 64 });
		opening.PushBack({ 448 + 384, 256 - 10, 64, 64 });
		opening.speed = 10.F;
		opening.loop = false;
		break; 

	default: 
		break; 
	}
	
	// Ready to add a "Boss Chest" animation 

	if (isOpened)
	{
		opening.SetCurrentFrame(opening.GetSize());
		currentAnimation = &opening;
	}
	else
		currentAnimation = &idle;

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

		if (!isOpened)
		{
			ChestTrigger* myTrigger = dynamic_cast<ChestTrigger*>(App->entityFactory->CreateTrigger(TRIGGER_TYPE::CHEST, GetPivotPos().x, GetPivotPos().y));
			if (myTrigger != nullptr)
				myTrigger->AssignOwner(this);
		}
		isAssigned = true;
	}

	if (toSpawnLoot && currentAnimation->Finished())
	{
		SpawnLoot();
		toSpawnLoot = false;
	}

	return true;
}

void ChestAsset::Draw()
{
	if (entityTex != nullptr) // if we have any specific linked texture
	{
		App->render->Blit(entityTex, position.x, position.y, &currentAnimation->GetCurrentFrame());
	}
	else
		App->render->Blit(App->entityFactory->assetsAtlasTex, position.x, position.y, &currentAnimation->GetCurrentFrame());
}

bool ChestAsset::Load(pugi::xml_node &)
{
	return true;
}

bool ChestAsset::Save(pugi::xml_node &node) const
{
	node.append_attribute("posX") = (int)position.x;
	node.append_attribute("posY") = (int)position.y;
	node.append_attribute("chestType") = (int)type;
	node.append_attribute("open") = isOpened;
	return true;
}

void ChestAsset::OpenChest()
{
	toSpawnLoot = true;
	currentAnimation = &opening;
	App->audio->PlayFx(App->scene->OpenChestSFX, 0);
	//SpawnLoot();
	// TODO: Add SFX
}

void ChestAsset::SpawnLoot()
{
	switch (type)
	{
	case ChestType::BOSS:
		App->entityFactory->RepeatAmountofConsumables(35, GetPivotPos(), OBJECT_TYPE::GOLD);
		App->entityFactory->RandomAmountofLoot(LOOT_TYPE::EQUIPABLE, 8, GetPivotPos());
		App->entityFactory->CreateTrigger(TRIGGER_TYPE::PORTAL, App->map->SubTileMapToWorld(82, 208).x, App->map->SubTileMapToWorld(82, 208).y, SceneState::WIN, White);
		break;

	case ChestType::NORMAL:
		App->entityFactory->RepeatAmountofConsumables(7, GetPivotPos(), OBJECT_TYPE::GOLD);
		App->entityFactory->RandomAmountofLoot(LOOT_TYPE::EQUIPABLE, 1, GetPivotPos());
		break;

	case ChestType::SILVER:
		App->entityFactory->RepeatAmountofConsumables(15, GetPivotPos(), OBJECT_TYPE::GOLD);
		App->entityFactory->RepeatAmountofConsumables(1, GetPivotPos(), OBJECT_TYPE::PHOENIX_TAIL);
		App->entityFactory->RandomAmountofLoot(LOOT_TYPE::EQUIPABLE, 3, GetPivotPos());
		if (App->entityFactory->CreateRandomBetween(0, 3) == 1)
			App->entityFactory->CreateLegendariEquipable(GetPivotPos(), EQUIPABLE_TYPE::NO_EQUIPABLE);

		break;

	default:
		break;
	}
	
	isOpened = true;
}
