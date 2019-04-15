#include "ConsumableLoot.h"
#include "j1Entity.h"



Consumable::Consumable(int posX, int posY) : LootEntity(LOOT_TYPE::CONSUMABLE, posX, posY)
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
		objectType = OBJECT_TYPE::GOLD;
	}

	else if (7 <  chance <= 10)
	{
		objectType = OBJECT_TYPE::POTIONS;
		if (chance <= 10)
		{
			consumableType = CONSUMABLE_TYPE::POTION;
			//other Potions
		}
	}

}

void Consumable::SetConsumable()
{
	ChooseConsumable();
	switch (objectType)
	{
	case OBJECT_TYPE::GOLD:


		loot_rect = { 13,27,22,23 };
		size.create(22, 23);

		SetPivot(16, 20);

		break;

	case OBJECT_TYPE::POTIONS:

		switch (consumableType)
		{
		case CONSUMABLE_TYPE::POTION:
			loot_rect = { 60,32,10,12 };

			size.create(10, 12);
			SetPivot(5, 10);
			break;
		default:
			break;
		}
		break;

	default:
		break;
	}

}

