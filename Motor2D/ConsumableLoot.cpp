#include "ConsumableLoot.h"
#include "j1Entity.h"
#include "easing.h"
#include "j1EntityFactory.h"
#include "j1Render.h"

Consumable::Consumable(int posX, int posY) : LootEntity(LOOT_TYPE::CONSUMABLE, posX, posY)
{
	
	SetConsumable();
	originPos.x = position.x;
	start = true;
	checkgrounded = true;
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
		else if (!repositionDescription)
		{
			if (objectType != OBJECT_TYPE::GOLD)
			{
				this->MyDescription->RepositionAllElements(App->render->WorldToScreen(this->position.x, this->position.y));   // what here?? :/
				repositionDescription = true;
			}
		}
		else
		{
			grounded = true;
			
		}

		if (checkgrounded && grounded)
		{
			checkgrounded = false;
			App->audio->PlayFx(App->entityFactory->lootGroundSFX, 0);
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
		objectType = OBJECT_TYPE::POTIONS;
		consumableType = CONSUMABLE_TYPE::POTION;

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

