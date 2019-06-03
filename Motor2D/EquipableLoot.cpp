#include "EquipableLoot.h"
#include "j1Entity.h"
#include "j1EntityFactory.h"

Equipable::Equipable(int posX, int posY) : LootEntity(LOOT_TYPE::EQUIPABLE, posX, posY)
{
	this->equipableType = EQUIPABLE_TYPE::NO_EQUIPABLE;
	SetEquipable();
	manualCollectable = false;
	entityTex = App->entityFactory->lootItemsTex;

	
	App->easing->CreateSpline(&position.x, App->map->SubTileMapToWorld(groundSubtileDestination.x, groundSubtileDestination.y).x - pivot.x, 2000, TypeSpline::EASE, std::bind(&LootEntity::CheckGrounded, this));

	App->easing->CreateSpline(&position.y, (App->map->SubTileMapToWorld(groundSubtileDestination.x, groundSubtileDestination.y).y - pivot.y)
		- App->map->SubTileMapToWorld(groundSubtileDestination.x, groundSubtileDestination.y).DistanceTo((iPoint)position) * 0.6,
		1000, TypeSpline::EASE_OUT_CUBIC, std::bind(&LootEntity::SetSplineToFall, this));

}

Equipable::Equipable(int posX, int posY, EQUIPABLE_TYPE OBJ_TYPE):LootEntity(LOOT_TYPE::EQUIPABLE, posX, posY)
{
	
	ToSelectLootFunction(OBJ_TYPE);
	equipableType = OBJ_TYPE;
	manualCollectable = false;
	entityTex = App->entityFactory->lootItemsTex;

	

	App->easing->CreateSpline(&position.x, App->map->SubTileMapToWorld(groundSubtileDestination.x, groundSubtileDestination.y).x - pivot.x, 2000, TypeSpline::EASE, std::bind(&LootEntity::CheckGrounded, this));

	App->easing->CreateSpline(&position.y, (App->map->SubTileMapToWorld(groundSubtileDestination.x, groundSubtileDestination.y).y-pivot.y)
		- App->map->SubTileMapToWorld(groundSubtileDestination.x, groundSubtileDestination.y).DistanceTo((iPoint)position)*0.6,
		1000, TypeSpline::EASE_OUT_CUBIC, std::bind(&LootEntity::SetSplineToFall, this));
}


Equipable::~Equipable()
{
	LOG("LootEquipment destoyed");
}


bool Equipable::Update(float dt)
{
	if (grounded)
	{
		App->audio->PlayFx(App->scene->lootGroundSFX, 0);
		grounded = false;
	}
	//if (App->entityFactory->player->selectedCharacterEntity->IsAiming())
		//CheckClampedCrossHairToSpawnDescription();

	return true;
}



EQUIPABLE_TYPE Equipable::ChooseEquipable()
{

	switch (GetRandomValue(1, 6))
	{
		LOG("");
	case 1:
		equipableType = EQUIPABLE_TYPE::SWORD;
		return EQUIPABLE_TYPE::SWORD;

	case 2:
		equipableType = EQUIPABLE_TYPE::ROD;
		return EQUIPABLE_TYPE::ROD;

	case 3:
		equipableType = EQUIPABLE_TYPE::BOW;
		return EQUIPABLE_TYPE::BOW;

	case 4:
		equipableType = EQUIPABLE_TYPE::ARMOR;
		return EQUIPABLE_TYPE::ARMOR;

	case 5:
		equipableType = EQUIPABLE_TYPE::VEST;
		return EQUIPABLE_TYPE::VEST;

	case 6:
		equipableType = EQUIPABLE_TYPE::MANTLE;
		return EQUIPABLE_TYPE::MANTLE;

	}
	return EQUIPABLE_TYPE::NO_EQUIPABLE;

}


void Equipable::SetEquipable()
{
	if(equipableType == EQUIPABLE_TYPE::NO_EQUIPABLE)
		ChooseEquipable();
	switch (equipableType)
	{
	case EQUIPABLE_TYPE::SWORD:
		objectType = OBJECT_TYPE::WEAPON_OBJECT;
		loot_rect = { 64, 96, 32, 32 };
		SetPivot(16, 28);
		size.create(32, 32);
		//353, 96
		break;

	case EQUIPABLE_TYPE::BOW:
		objectType = OBJECT_TYPE::WEAPON_OBJECT;
		loot_rect = { 194, 770, 32, 32 };
		SetPivot(16, 16);
		size.create(32, 32);
		//34,834
		break;

	case EQUIPABLE_TYPE::ROD:
		objectType = OBJECT_TYPE::WEAPON_OBJECT;
		loot_rect = { 96, 736, 32, 32 };
		SetPivot(16, 28);
		size.create(32, 32);
		//33 , 738
		break;

	case EQUIPABLE_TYPE::ARMOR:
		objectType = OBJECT_TYPE::ARMOR_OBJECT;
		loot_rect = { 128, 932, 32, 32 };
		SetPivot(16, 28);
		size.create(32, 32);
		//128 , 964
		break;

	case EQUIPABLE_TYPE::VEST:
		objectType = OBJECT_TYPE::ARMOR_OBJECT;
		loot_rect = { 254, 1441, 32, 32};
		SetPivot(16, 28);
		size.create(32, 32);
		//95,1409
		break;


	case EQUIPABLE_TYPE::MANTLE:
		objectType = OBJECT_TYPE::ARMOR_OBJECT;
		loot_rect = { 350, 1409, 32, 32 };
		SetPivot(16, 28);
		size.create(32, 32);
		//leg mantle x446,y1408

		break;

	}
}

void Equipable::ToSelectLootFunction(EQUIPABLE_TYPE type)
{
	switch (type)
	{
	case EQUIPABLE_TYPE::SWORD:
		objectType = OBJECT_TYPE::WEAPON_OBJECT;
		loot_rect = { 64, 96, 32, 32 };
		SetPivot(16, 28);
		size.create(32, 32);
		break;

	case EQUIPABLE_TYPE::BOW:
		objectType = OBJECT_TYPE::WEAPON_OBJECT;
		loot_rect = { 194, 770, 32, 32 };
		SetPivot(16, 16);
		size.create(32, 32);
		break;

	case EQUIPABLE_TYPE::ROD:
		objectType = OBJECT_TYPE::WEAPON_OBJECT;
		loot_rect = { 96, 736, 32, 32 };
		SetPivot(16, 28);
		size.create(32, 32);
		break;

	case EQUIPABLE_TYPE::ARMOR:
		objectType = OBJECT_TYPE::ARMOR_OBJECT;
		loot_rect = { 128, 932, 32, 32 };
		SetPivot(16, 28);
		size.create(32, 32);
		break;

	case EQUIPABLE_TYPE::VEST:
		objectType = OBJECT_TYPE::ARMOR_OBJECT;
		loot_rect = { 254, 1441, 32, 32 };
		SetPivot(16, 28);
		size.create(32, 32);
		break;

	case EQUIPABLE_TYPE::MANTLE:
		objectType = OBJECT_TYPE::ARMOR_OBJECT;
		loot_rect = { 350, 1409, 32, 32 };
		SetPivot(16, 28);
		size.create(32, 32);
		break;

	}
}
