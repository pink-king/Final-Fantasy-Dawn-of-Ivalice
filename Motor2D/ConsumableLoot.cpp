#include "ConsumableLoot.h"
#include "j1Entity.h"
#include "j1LootManager.h"


Consumable::Consumable(int posX, int posY) : LootEntity(posX, posY)
{
		
	SetConsumable();
}


Consumable::~Consumable()
{

}


bool Consumable::Update(float dt)
{
	return true;
}



void Consumable::ChooseConsumable()
{
	chance = GetRandomValue(1, 10);
	LOG("chooseing consumable");
	if (chance <= 7)
	{
		LOG("consumable rand");
		consumableType = CONSUMABLE_TYPE::GOLD;
	}

	else if( 7 <  chance <=10)
	{

		consumableType = CONSUMABLE_TYPE::POTION;
	}

}

void Consumable::SetConsumable()
{
	ChooseConsumable();
	switch (consumableType)
	{
	case CONSUMABLE_TYPE::GOLD:
		

		loot_rect = { 13,27,22,23 };
		size.create(22, 23);

		SetPivot(16, 20);

		break;

	case CONSUMABLE_TYPE::POTION:

		loot_rect = { 60,32,10,12 };

		size.create(10, 12);
		SetPivot(5, 10);
		break;
	}

}

