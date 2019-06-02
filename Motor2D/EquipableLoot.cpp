#include "EquipableLoot.h"
#include "j1Entity.h"
#include "j1EntityFactory.h"
#include "j1EasingSplines.h"

Equipable::Equipable(int posX, int posY) : LootEntity(LOOT_TYPE::EQUIPABLE, posX, posY)
{
	this->equipableType = EQUIPABLE_TYPE::NO_EQUIPABLE;
	SetEquipable();
	originPos.x = position.x;
	originPos.y = position.y;

	
	start = true;
	checkgrounded = true;
	manualCollectable = false;
	entityTex = App->entityFactory->lootItemsTex;

	

	App->easing->CreateSpline(&position.x, App->map->MapToWorld(groundTileDestination.x, groundTileDestination.y).x, 1000 ,TypeSpline::EASE);
	App->easing->CreateSpline(&position.y, App->map->MapToWorld(groundTileDestination.x, groundTileDestination.y).y, 1000, TypeSpline::EASE_OUT_BOUNCE);

}

Equipable::Equipable(int posX, int posY, EQUIPABLE_TYPE OBJ_TYPE):LootEntity(LOOT_TYPE::EQUIPABLE, posX, posY)
{
	ToSelectLootFunction(OBJ_TYPE);
	equipableType = OBJ_TYPE;
	originPos.x = position.x;
	originPos.y = position.y;
	start = true;
	checkgrounded = true;
	manualCollectable = false;
	entityTex = App->entityFactory->lootItemsTex;

	

	App->easing->CreateSpline(&position.x, App->map->MapToWorld(groundTileDestination.x, groundTileDestination.y).x, 1000, TypeSpline::EASE);
	App->easing->CreateSpline(&position.y, App->map->MapToWorld(groundTileDestination.x, groundTileDestination.y).y, 1000, TypeSpline::EASE_OUT_BOUNCE);
}


Equipable::~Equipable()
{
	LOG("LootEquipment destoyed");
}


bool Equipable::Update(float dt)
{
	
	//if (start)
	//{
	//	goalPos = SetDestinationPos(goalPos.x, goalPos.y);
	//	start = false;
	//	position.y + 5;

	//	DecideExplosion();
	//}

	//dt = EaseOutBack(displacementTime.ReadMs())*0.000001; 

	//if (displacementTime.ReadMs() <= 280)
	//{
	//	ExplosionMaker(dt);
	//	/*LOG("displaced %f", position.x - originPos.x);
	//	LOG("actual time %f", timeTest);*/
	//}
	//else grounded = true;
	/*else if(!repositionDescription)
	{
		this->MyDescription->RepositionAllElements(App->render->WorldToScreen(this->GetPosition().x, this->GetPosition().y));   // what here?? :/
		repositionDescription = true; 
	}*/
	//float timepassed = SDL_GetTicks() - timeStarted;
	//GetDistanceTotravel();
	//if (timepassed < 1000)
	//{
	//	LOG("preEase posX %f", position.x);
	//	
	//	position.x = Ease(timepassed, originPos.x, distanceTotravel.x, 1000);
	//	LOG("Eased posX %f", position.x);
	//	LOG("originPos.x %f", originPos.y);
	//	LOG("DistanceToTravel.x %i", distanceTotravel.x);
	//}

	//if (timepassed < 1000)
	//{
	//	LOG("preEase posY %f", position.y);
	//	position.y =  EaseOutBounce(timepassed, originPos.y, distanceTotravel.y, 1000);
	//	LOG("Eased posY %f", position.y);
	//	LOG("originPos.y %f", originPos.y);
	//	LOG("DistanceToTravel.y %i", distanceTotravel.y);
	//}
	//if (timepassed < 1000)
	//{
	////position.y = Ease(timepassed, originPos.y, distanceTotravel.y, 1500);
	//position.y = EaseOutCubic(timepassed, originPos.y, distanceTotravel.y, 900);

	//}
	

	
	if (checkgrounded && grounded)
	{
		checkgrounded = false;
		App->audio->PlayFx(App->scene->lootGroundSFX, 0);
	}
	//if (App->entityFactory->player->selectedCharacterEntity->IsAiming())
		//CheckClampedCrossHairToSpawnDescription();

	return true;
}

//bool Equipable::CleanUp()
//{
//	return true;
//}

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
		loot_rect = { 254, 1441, 32, 32};
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
