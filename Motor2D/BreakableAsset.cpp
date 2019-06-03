#include "BreakableAsset.h"
#include "j1Render.h"
#include "j1EntityFactory.h"
#include "j1PathFinding.h"

BreakableAsset::BreakableAsset(const iPoint & pos, BreakableType type, bool isBroken) : isBroken(isBroken)
{
	switch (type)
	{
		// Rects -9 in Y to adjust to tile's center
	case BreakableType::JAR:
		idle.PushBack({ 384, 693, 64, 64 });

		breaking.PushBack({ 448, 693, 64, 64 });
		breaking.PushBack({ 512, 693, 64, 64 });
		breaking.PushBack({ 576, 693, 64, 64 });
		breaking.PushBack({ 640, 693, 64, 64 });
		breaking.speed = 10.f;
		breaking.loop = false; 

		break;

	case BreakableType::JARFULL:
		idle.PushBack({ 384, 435, 64,64 });

		breaking.PushBack({ 512, 693, 64, 64 });
		breaking.PushBack({ 576, 693, 64, 64 });
		breaking.PushBack({ 640, 693, 64, 64 });
		breaking.speed = 10.f;
		breaking.loop = false;
		break;

	case BreakableType::BARREL:
		idle.PushBack({ 384, 757, 64, 64 });
							  
		breaking.PushBack({ 448, 757, 64, 64 });
		breaking.PushBack({ 512, 757, 64, 64 });
		breaking.PushBack({ 576, 757, 64, 64 });  
		breaking.PushBack({ 640, 757, 64, 64 });
		breaking.speed = 10.f;
		breaking.loop = false;
		break;

	default:
		LOG("Not Correct Breakable Asset Constructed");
		break;
	}
	
	if (isBroken)
	{
		breaking.SetCurrentFrame(breaking.GetSize());
		currentAnimation = &breaking;
	}
	else
		currentAnimation = &idle; 

	position.x = pos.x;
	position.y = pos.y;

	pivot.create(32, 44);
	size.create(64, 64);

	this->type = BREAKABLE_ASSET;
	this->life = 1;
	// When this entity is created, the Walkability Map is not created yet
	//	App->pathfinding->ActivateTile(App->map->WorldToMap(GetPivotPos().x, GetPivotPos().y));
}

BreakableAsset::~BreakableAsset()
{
}



bool BreakableAsset::PreUpdate()
{

	//if (!isAssigned)	// I have to do this the pretty way: booleans! :D
	//{
	//	App->pathfinding->ActivateTile(App->map->WorldToMap(GetPivotPos().x, GetPivotPos().y));
	//	isAssigned = true;
	//}

	return true;
}

bool BreakableAsset::Update(float dt)
{

	if (to_die && !isBroken)
		Break();

	return true;
}

void BreakableAsset::Draw()
{

	if (currentAnimation != nullptr)
	{
		if (entityTex != nullptr) // if we have any specific linked texture
		{
			App->render->Blit(entityTex, position.x, position.y, &currentAnimation->GetCurrentFrame(), 1.0F, flip);
		}
		else
			App->render->Blit(App->entityFactory->assetsAtlasTex, position.x, position.y, &currentAnimation->GetCurrentFrame());
	}
	else App->render->Blit(App->entityFactory->assetsAtlasTex, position.x, position.y, &drawAtlasRect);

}

void BreakableAsset::Break()
{
	isBroken = true;
	currentAnimation = &breaking;
	App->entityFactory->RepeatAmountofConsumables(4, GetPivotPos(), OBJECT_TYPE::GOLD); 
	App->entityFactory->RepeatAmountofConsumables(1, GetPivotPos(), OBJECT_TYPE::POTIONS);

	// TODO: Add SFX

	//App->pathfinding->DeactivateTile(App->map->WorldToMap(GetPivotPos().x, GetPivotPos().y));
}
