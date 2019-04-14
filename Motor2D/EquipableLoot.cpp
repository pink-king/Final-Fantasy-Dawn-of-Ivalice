#include "EquipableLoot.h"
#include "j1Entity.h"
#include "j1LootManager.h"



Equipable::Equipable(int posX, int posY) : LootEntity(LOOT_TYPE::EQUIPABLE, posX, posY)
{
	SetEquipable();
	originPos.x = position.x;
	start = true;
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
	return true;
}

//bool Equipable::CleanUp()
//{
//	return true;
//}

EQUIPABLE_TYPE Equipable::ChooseEquipable()
{
	equipableChance = GetRandomValue(1, 100);

	if (equipableChance <= 33)
	{
		equipableType = EQUIPABLE_TYPE::SWORD;
		return EQUIPABLE_TYPE::SWORD;
	}
	else if (33 < equipableChance <= 66)
	{
		equipableType = EQUIPABLE_TYPE::BOW;
		return EQUIPABLE_TYPE::BOW;
	}
	else if(equipableChance > 66)
	{
		equipableType = EQUIPABLE_TYPE::ROD;
		return EQUIPABLE_TYPE::ROD;

	}

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
		SetPivot(18, 75);

		size.create(5, 13);
		break;
	case EQUIPABLE_TYPE::ARMOR:
		objectType = OBJECT_TYPE::ARMOR_OBJECT;
		break;
	case EQUIPABLE_TYPE::VEST:
		objectType = OBJECT_TYPE::ARMOR_OBJECT;

		break;
	case EQUIPABLE_TYPE::MANTLE:
		objectType = OBJECT_TYPE::ARMOR_OBJECT;

		break;
	}
}