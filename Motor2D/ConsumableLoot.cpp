#include "ConsumableLoot.h"
#include "j1App.h"
#include "j1Entity.h"
#include "j1Scene.h"
#include "easing.h"
#include "j1EntityFactory.h"

Consumable::Consumable(int posX, int posY) : LootEntity(LOOT_TYPE::CONSUMABLE, posX, posY)
{
	SetConsumable();
	originPos.x = position.x;
	start = true;
	checkgrounded = true;
	manualCollectable = true;
}

Consumable::Consumable(int posX, int posY, OBJECT_TYPE objectT, CONSUMABLE_TYPE consumableT) : LootEntity(LOOT_TYPE::CONSUMABLE, posX, posY)
{
	originPos.x = position.x;
	start = true;
	checkgrounded = true;
	manualCollectable = true;
	objectType = objectT;
	consumableType = consumableT;
}


Consumable::~Consumable()
{
	
}

bool Consumable::Start()
{
	
	return true;
}
bool Consumable::Update(float dt)
{
	
		if (start)
		{
			goalPos = SetDestinationPos(goalPos.x, goalPos.y);
			start = false;
			
			
			DecideExplosion();
		}

		dt = EaseOutBack(displacementTime.ReadMs())*0.000001;

		if (displacementTime.ReadMs() <= 280)
		{
			ExplosionMaker(dt);
			LOG("displaced %i",position.x - originPos.x);
			LOG("actual time %f", timeTest);
		}
		else
		{
			grounded = true;
			
		}
		/*else if (!repositionDescription)
		{
			this->MyDescription->RepositionAllElements(App->render->WorldToScreen(this->GetPosition().x, this->GetPosition().y));   // what here?? :/
			repositionDescription = true;
		}*/

		if (checkgrounded && grounded)
		{
			checkgrounded = false;
			if (objectType == OBJECT_TYPE::POTIONS)
				App->audio->PlayFx(App->entityFactory->potionGroundSFX, 0);

			else if(objectType == OBJECT_TYPE::GOLD) App->audio->PlayFx(App->entityFactory->coinGroundedSFX, 0);
		}
    
	return true;
}




void Consumable::ChooseConsumable()
{
	
	if (App->entityFactory->justGold)
	{
		objectType = OBJECT_TYPE::GOLD;
		App->entityFactory->justGold = false;
	}
	else
	{
		int randNum = (GetRandomValue(1, 6));
		if (randNum == 1)
		{
			objectType = OBJECT_TYPE::POTIONS;
			consumableType = CONSUMABLE_TYPE::POTION;
		}
		else if (randNum == 2)
		{
			objectType = OBJECT_TYPE::PHOENIX_TAIL;
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

		SetPivot(12, 17);

		break;

	case OBJECT_TYPE::POTIONS:

		switch (consumableType)
		{
		case CONSUMABLE_TYPE::POTION:
			loot_rect = { 60,32,10,12 };
			lootname.assign("potion");
			name.assign("potion");
			size.create(10, 12);
			SetPivot(5, 10);
			break;
		default:
			break;
		}
		break;

	case OBJECT_TYPE::PHOENIX_TAIL:
		loot_rect = { 149,30,13,16 };
		lootname.assign("phoenixTail");
		name.assign("phoenixTail");
		size.create(13, 16);
		SetPivot(7, 14);
		break;

	default:
		break;
	}

}

