#include "EquipableLoot.h"
#include "j1Entity.h"




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
	equipableChance = GetRandomValue(1, 3);
	LOG("equipable rand %i", equipableChance);
	if (equipableChance == 1)
	{
		equipableType = EQUIPABLE_TYPE::SWORD;
		return EQUIPABLE_TYPE::SWORD;
	}
	else if (equipableChance == 2)
	{
		
		equipableType = EQUIPABLE_TYPE::ROD;
		
		return EQUIPABLE_TYPE::ROD;
	}
	else if(equipableChance == 3)
	{
		equipableType = EQUIPABLE_TYPE::BOW;
		return EQUIPABLE_TYPE::BOW;

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
		SetPivot(6, 8);

		size.create(11, 16);
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