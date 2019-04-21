#include "EntityArrow.h"
#include "j1App.h"
#include "j1Textures.h"
#include "j1EntityFactory.h"
#include "j1AttackManager.h"
#include "j1Render.h"
#include "j1PathFinding.h"
#include "p2Defs.h"

EntityArrow::EntityArrow(fPoint pos, fPoint destination, uint speed, const j1Entity* owner) : destination(destination), speed(speed), owner(owner), j1Entity(ENTITY_TYPE::ENEMY02, pos.x, pos.y, "Arrow")
{
	entityTex = App->tex->Load("textures/spells/Ritz_attacks/Ritz_fx.png");
	debugSubtile = App->entityFactory->debugsubtileTex;

	SetPivot(14, 4);
	size.create(26, 8);

	direction = destination - GetPivotPos();
	direction.Normalize(); 
	angle = SetMyAngleRotation(direction);

	drawAtlasRect = { 9, 28, 26,8 };
}

EntityArrow::~EntityArrow()
{
	LOG("Bye arrow!");
}

bool EntityArrow::PreUpdate()
{
	CheckMyPos(); 
	if (CollisionWithWall()) {
		to_delete = true; 
	}
	
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

	if (TooFarAway())	{
		to_delete = true; 
	}

	return true;
}

bool EntityArrow::Contact()
{
	App->attackManager->AddPropagationAttack(owner, imOnSubtile, propagationType::BFS, 5, 7, 50);
	to_delete = true; 

	return true;
}

float EntityArrow::SetMyAngleRotation(const fPoint & direction)
{
	return RADTODEG * atan2f(direction.y, direction.x); 
}

bool EntityArrow::TooFarAway() const
{
	// It checks its distance with the player, so if he moves towards it, the projectile won't delete
	return (App->entityFactory->player->GetPivotPos().DistanceManhattan(GetPivotPos()) > 720);
}

bool EntityArrow::CollisionWithWall() const
{
	return (!App->pathfinding->IsWalkable(GetTilePos()));
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
			App->render->Blit(entityTex, position.x, position.y, &drawAtlasRect, 1.0F, SDL_FLIP_NONE, 1.0F, angle, pivot.x, pivot.y);
	}	
}


