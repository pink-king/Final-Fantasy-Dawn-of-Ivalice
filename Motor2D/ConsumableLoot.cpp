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

	entityTex = App->entityFactory->lootItemsTex;
}

Consumable::Consumable(int posX, int posY, OBJECT_TYPE objectT, CONSUMABLE_TYPE consumableT) : LootEntity(LOOT_TYPE::CONSUMABLE, posX, posY)
{
	SetConsumable(CONSUMABLE_TYPE::POTION);
	originPos.x = position.x;
	start = true;
	checkgrounded = true;
	manualCollectable = true;
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

			//else if(objectType == OBJECT_TYPE::PHOENIX_TAIL)
		}
    
	return true;
}




void Consumable::ChooseConsumable(CONSUMABLE_TYPE type)
{
	
	if (App->entityFactory->justGold)
	{
		objectType = OBJECT_TYPE::GOLD;
		App->entityFactory->justGold = false;
	}
	else
	{
		int randNum = (GetRandomValue(1, 8));
		if (randNum <= 2)
		{
		
				objectType = OBJECT_TYPE::POTIONS;
				consumableType = CONSUMABLE_TYPE::POTION; 
		
		}
		else if(randNum == 3)
		{
			objectType = OBJECT_TYPE::PHOENIX_TAIL;
		}
		else
		{
			to_delete = true;
		}
	}
	
}

void Consumable::SetConsumable(CONSUMABLE_TYPE type)
{
	if (type == CONSUMABLE_TYPE::POTION)
	{
		objectType = OBJECT_TYPE::POTIONS;
		consumableType = CONSUMABLE_TYPE::POTION;
	}
	else
	{
		ChooseConsumable();
	}

	switch (objectType)
	{
	case OBJECT_TYPE::GOLD:


		loot_rect = { 420,1611,32,32 };
		size.create(32, 32);

		SetPivot(16, 28);

		break;

	case OBJECT_TYPE::POTIONS:

		switch (consumableType)
		{
		case CONSUMABLE_TYPE::POTION:
			loot_rect = { 416,1569,32,32 };
			lootname.assign("potion");
			name.assign("potion");
			size.create(32, 32);
			SetPivot(16, 16);
			break;
		default:
			break;
		}
		break;

	case OBJECT_TYPE::PHOENIX_TAIL:
		loot_rect = { 448,1570,32,32 };
		lootname.assign("phoenixTail");
		name.assign("phoenixTail");
		size.create(32, 32);
		SetPivot(16, 30);
		break;

	default:
		break;
	}

}

