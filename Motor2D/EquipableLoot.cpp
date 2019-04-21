#include "EquipableLoot.h"
#include "j1Entity.h"
#include "j1EntityFactory.h"


Equipable::Equipable(int posX, int posY) : LootEntity(LOOT_TYPE::EQUIPABLE, posX, posY)
{
	SetEquipable();
	originPos.x = position.x;
	start = true;
	checkgrounded = true;
}


Equipable::~Equipable()
{
	LOG("LootEquipment destoyed");
}


bool Equipable::Update(float dt)
{
	if (start)
	{
		goalPos = SetDestinationPos(goalPos.x, goalPos.y);
		start = false;
		position.y + 5;

		DecideExplosion();
	}

	dt = EaseOutBack(displacementTime.ReadMs())*0.000001; 

	if (displacementTime.ReadMs() <= 280)
	{
		ExplosionMaker(dt);
		LOG("displaced %f", position.x - originPos.x);
		LOG("actual time %f", timeTest);
	}
	else grounded = true;
	/*else if(!repositionDescription)
	{
		this->MyDescription->RepositionAllElements(App->render->WorldToScreen(this->GetPosition().x, this->GetPosition().y));   // what here?? :/
		repositionDescription = true; 
	}*/


	/*if (!spawnedDescription)
	{*/
		
	//}

	
	if (checkgrounded && grounded)
	{
		checkgrounded = false;
		App->audio->PlayFx(App->entityFactory->lootGroundSFX, 0);
	}

	CheckClampedCrossHairToSpawnDescription();

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

	switch (ChooseEquipable())
	{
	case EQUIPABLE_TYPE::SWORD:
		objectType = OBJECT_TYPE::WEAPON_OBJECT;
		loot_rect = { 59,67,16,16 };
		SetPivot(8, 12);
		size.create(16, 16);
		break;

	case EQUIPABLE_TYPE::BOW:
		objectType = OBJECT_TYPE::WEAPON_OBJECT;
		loot_rect = { 93,32,13,13 };
		SetPivot(7, 9);
		size.create(13, 13);
		break;

	case EQUIPABLE_TYPE::ROD:
		objectType = OBJECT_TYPE::WEAPON_OBJECT;
		loot_rect = { 12,67,11,16 };
		SetPivot(6, 8);
		size.create(11, 16);
		break;

	case EQUIPABLE_TYPE::ARMOR:
		objectType = OBJECT_TYPE::ARMOR_OBJECT;
		loot_rect = { 93,69,15,15 };
		SetPivot(7, 10);
		size.create(15, 15);
		break;

	case EQUIPABLE_TYPE::VEST:
		objectType = OBJECT_TYPE::ARMOR_OBJECT;
		loot_rect = { 123,32,14,14 };
		SetPivot(7, 9);
		size.create(14, 14);
		break;

	case EQUIPABLE_TYPE::MANTLE:
		objectType = OBJECT_TYPE::ARMOR_OBJECT;
		loot_rect = { 122,70,14,14 };
		SetPivot(7, 9);
		size.create(14, 14);
		break;

	}
}