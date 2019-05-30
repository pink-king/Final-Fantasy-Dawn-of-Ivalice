#include "BreakableAsset.h"
#include "j1Render.h"
#include "j1EntityFactory.h"
#include "j1PathFinding.h"

BreakableAsset::BreakableAsset(const iPoint & pos, BreakableType type)
{
	switch (type)
	{
	case BreakableType::JAR:
		idle.PushBack({ 384, 702, 64, 64 });

		breaking.PushBack({ 448, 702, 64, 64 });
		breaking.PushBack({ 512, 702, 64, 64 });
		breaking.PushBack({ 576, 702, 64, 64 });
		breaking.PushBack({ 640, 702, 64, 64 });
		breaking.speed = 10.f;
		breaking.loop = false; 

		break;

	case BreakableType::JARFULL:
		idle.PushBack({ 384, 444, 64,64 });

		breaking.PushBack({ 512, 702, 64, 64 });
		breaking.PushBack({ 576, 702, 64, 64 });
		breaking.PushBack({ 640, 702, 64, 64 });
		breaking.speed = 10.f;
		breaking.loop = false;
		break;

	case BreakableType::BARREL:
		idle.PushBack({ 384, 766, 64, 64 });
							  
		breaking.PushBack({ 448, 766, 64, 64 });
		breaking.PushBack({ 512, 766, 64, 64 });
		breaking.PushBack({ 576, 766, 64, 64 });  
		breaking.PushBack({ 640, 766, 64, 64 });
		breaking.speed = 10.f;
		breaking.loop = false;
		break;

	default:
		LOG("Not Correct Breakable Asset Constructed");
		break;
	}

	currentAnimation = &idle;

	position.x = pos.x;
	position.y = pos.y;

	pivot.create(32, 44);
	size.create(64, 64);

	this->type = BREAKABLE_ASSET;
	this->life = 1;
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
	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
		to_die = true; 

	if (to_die && !isBroken)
		Break();

	return true;
}

bool BreakableAsset::CleanUp()
{
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
	App->entityFactory->RandomAmountofLoot(LOOT_TYPE::CONSUMABLE, 4, GetPivotPos());
	App->entityFactory->RandomAmountofLoot(LOOT_TYPE::EQUIPABLE, 1, GetPivotPos());
	// TODO: Add SFX

	//App->pathfinding->DeactivateTile(App->map->WorldToMap(GetPivotPos().x, GetPivotPos().y));
}
