#include "ConsumableLoot.h"
#include "j1App.h"
#include "j1Entity.h"
#include "j1Scene.h"
#include "easing.h"
#include "j1EntityFactory.h"

Consumable::Consumable(int posX, int posY) : LootEntity(LOOT_TYPE::CONSUMABLE, posX, posY)
{
	SetConsumable();

	manualCollectable = true;

	entityTex = App->entityFactory->lootItemsTex;

	App->easing->CreateSpline(&position.x, App->map->SubTileMapToWorld(groundSubtileDestination.x, groundSubtileDestination.y).x - pivot.x, 2000, TypeSpline::EASE);

	App->easing->CreateSpline(&position.y, (App->map->SubTileMapToWorld(groundSubtileDestination.x, groundSubtileDestination.y).y - pivot.y)
		- App->map->SubTileMapToWorld(groundSubtileDestination.x, groundSubtileDestination.y).DistanceTo((iPoint)position) * 0.6,
		1000, TypeSpline::EASE_OUT_CUBIC, std::bind(&LootEntity::SetSplineToFall, this));
}

Consumable::Consumable(int posX, int posY, OBJECT_TYPE objectT, CONSUMABLE_TYPE consumableT) : LootEntity(LOOT_TYPE::CONSUMABLE, posX, posY)
{
	SetConsumable(CONSUMABLE_TYPE::POTION);
	objectType = objectT;
	manualCollectable = true;

	entityTex = App->entityFactory->lootItemsTex;

	App->easing->CreateSpline(&position.x, App->map->SubTileMapToWorld(groundSubtileDestination.x, groundSubtileDestination.y).x - pivot.x, 2000, TypeSpline::EASE);

	App->easing->CreateSpline(&position.y, (App->map->SubTileMapToWorld(groundSubtileDestination.x, groundSubtileDestination.y).y - pivot.y)
		- App->map->SubTileMapToWorld(groundSubtileDestination.x, groundSubtileDestination.y).DistanceTo((iPoint)position) * 0.6,
		1000, TypeSpline::EASE_OUT_CUBIC, std::bind(&LootEntity::SetSplineToFall, this));
}
Consumable::Consumable(int posX, int posY, OBJECT_TYPE objectT) : LootEntity(LOOT_TYPE::CONSUMABLE, posX, posY)
{
	ToSelectLootFunction( objectT);
	objectType = objectT;
	manualCollectable = true;

	entityTex = App->entityFactory->lootItemsTex;

	App->easing->CreateSpline(&position.x, App->map->SubTileMapToWorld(groundSubtileDestination.x, groundSubtileDestination.y).x - pivot.x, 2000, TypeSpline::EASE);

	App->easing->CreateSpline(&position.y, (App->map->SubTileMapToWorld(groundSubtileDestination.x, groundSubtileDestination.y).y - pivot.y)
		- App->map->SubTileMapToWorld(groundSubtileDestination.x, groundSubtileDestination.y).DistanceTo((iPoint)position) * 0.6,
		1000, TypeSpline::EASE_OUT_CUBIC, std::bind(&LootEntity::SetSplineToFall, this));
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
	

		/*else if (!repositionDescription)
		{
			this->MyDescription->RepositionAllElements(App->render->WorldToScreen(this->GetPosition().x, this->GetPosition().y));   // what here?? :/
			repositionDescription = true;
		}*/

	if ((position.y == App->map->SubTileMapToWorld(groundSubtileDestination.x + 1, groundSubtileDestination.y - 1).y - pivot.y) && PlaySFXtime != 0)
	{
		if (objectType == OBJECT_TYPE::POTIONS)
			App->audio->PlayFx(App->scene->potionGroundSFX, 0);

		else if (objectType == OBJECT_TYPE::GOLD) App->audio->PlayFx(App->scene->coinGroundedSFX, 0);

		else if(objectType == OBJECT_TYPE::PHOENIX_TAIL) App->audio->PlayFx(App->scene->potionGroundSFX, 0);

		PlaySFXtime -= 1;
	}

	
    
	return true;
}

void Consumable::ToSelectLootFunction(OBJECT_TYPE type)
{
	switch (type)
	{
	case OBJECT_TYPE::GOLD:


		loot_rect = { 448,1568,32,32 };
		size.create(32, 32);

		SetPivot(16, 28);

		break;

	case OBJECT_TYPE::POTIONS:

			loot_rect = { 384,1568,32,32 };
			lootname.assign("potion");
			name.assign("potion");
			size.create(32, 32);
			SetPivot(16, 16);
		break;

	case OBJECT_TYPE::PHOENIX_TAIL:
		loot_rect = { 416,1568,32,32 };
		lootname.assign("phoenixTail");
		name.assign("phoenixTail");
		size.create(32, 32);
		SetPivot(16, 30);
		break;

	default:
		break;
	}
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


		loot_rect = { 448,1568,32,32 };
		size.create(32, 32);

		SetPivot(16, 28);

		break;

	case OBJECT_TYPE::POTIONS:

		switch (consumableType)
		{
		case CONSUMABLE_TYPE::POTION:
			loot_rect = { 384,1568,32,32 };
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
		loot_rect = { 416,1568,32,32 };
		lootname.assign("phoenixTail");
		name.assign("phoenixTail");
		size.create(32, 32);
		SetPivot(16, 30);
		break;

	default:
		break;
	}

}

