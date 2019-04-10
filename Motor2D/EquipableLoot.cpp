#include "EquipableLoot.h"
#include "j1Entity.h"
#include "j1LootManager.h"



Equipable::Equipable(int posX, int posY) : LootEntity(posX, posY)
{
	entityTex = App->lootManager->LootTexture;

	SetEquipable();
}


Equipable::~Equipable()
{
	LOG("LootEquipment destoyed");
}


bool Equipable::Update(float dt)
{

	return true;
}

//bool Equipable::CleanUp()
//{
//	return true;
//}

void Equipable::ChooseEquipable()
{
	equipableChance = GetRandomValue(1, 100);

	if (equipableChance <= 33)
	{
		equipable_type = EQUIPABLE_TYPE::SWORD;
	}
	else if (33 < equipableChance <= 66)
	{
		equipable_type = EQUIPABLE_TYPE::BOW;
	}
	else if (66 < equipableChance <= 100)
	{
		equipable_type = EQUIPABLE_TYPE::ROD;

	}
}


void Equipable::SetEquipable()
{

	ChooseEquipable();

	switch (equipable_type)
	{
	case EQUIPABLE_TYPE::SWORD:

		loot_rect = { 59,67,16,16 };
		SetPivot(67, 75);

		size.create(16, 16);
		break;
	
	case EQUIPABLE_TYPE::BOW:
		loot_rect = { 93,32,13,13 };
		SetPivot(100, 39);

		size.create(13, 13);
		break;
	case EQUIPABLE_TYPE::ROD:
		loot_rect = { 12,67,11,16 };
		SetPivot(18, 75);

		size.create(16, 16);
		break;
	case EQUIPABLE_TYPE::ARMOR:
		break;
	case EQUIPABLE_TYPE::VEST:
		break;
	case EQUIPABLE_TYPE::MANTLE:
		break;
	}
}
void Equipable::Draw()
{
}