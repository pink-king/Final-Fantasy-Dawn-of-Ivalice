#include "EntityArrow.h"
#include "j1App.h"
#include "j1Textures.h"
#include "j1EntityFactory.h"
#include "j1AttackManager.h"
#include "j1Render.h"

EntityArrow::EntityArrow(fPoint pos, fPoint destination, uint speed) : destination(destination), speed(speed), j1Entity(ENTITY_TYPE::ENEMY02, pos.x, pos.y, "Arrow")
{
	direction = destination - position;
	direction.Normalize(); 
	drawAtlasRect = { 9, 28, 26,8 };

	SetPivot(13, 4);
	size.create(26, 8);
	entityTex = App->tex->Load("textures/spells/Ritz_attacks/Ritz_fx.png");
	debugSubtile = App->entityFactory->debugsubtileTex; 
}

EntityArrow::~EntityArrow()
{
	LOG("Bye arrow!");
}

bool EntityArrow::PreUpdate()
{
	CheckMyPos(); 
	return true;
}

bool EntityArrow::Update(float dt)
{
	if (!to_explode) {
		Move(dt);
	}
	else Contact(); 

	return true;
}

bool EntityArrow::Move(float dt)
{
	position += direction * speed * dt; 
	return true;
}

bool EntityArrow::CheckMyPos()
{
	if (!App->entityFactory->isThisSubtileEnemyFree(GetSubtilePos())){
		to_explode = true; 
	}

	return true;
}

bool EntityArrow::Contact()
{
	const j1Entity* player = App->entityFactory->player->GetSelectedCharacterEntity();
	App->attackManager->AddPropagationAttack(player, imOnSubtile, propagationType::BFS, 5, 7, 50);
	to_delete = true; 

	return true;
}

bool EntityArrow::CleanUp()
{
	if (entityTex != nullptr)
	{
		App->tex->UnLoad(entityTex);
		entityTex = nullptr;
	}

	return true;
}

void EntityArrow::Draw()
{
	iPoint subTilePos = GetSubtilePos();
	subTilePos = App->map->SubTileMapToWorld(subTilePos.x, subTilePos.y);
	App->render->Blit(debugSubtile, subTilePos.x, subTilePos.y, NULL);

	if (entityTex != nullptr)
	{
		if (currentAnimation != nullptr)
			App->render->Blit(entityTex, position.x, position.y, &currentAnimation->GetCurrentFrame(), 1.0F, flip);
		else
			App->render->Blit(entityTex, position.x, position.y, &drawAtlasRect);
	}	
}


