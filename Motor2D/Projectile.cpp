#include "Projectile.h"
#include "j1EntityFactory.h"
#include "j1PathFinding.h"
#include "j1Render.h"
#include "j1Window.h"



Projectile::Projectile(fPoint pos, fPoint destination, uint speed, const j1Entity * owner, const char* name, PROJECTILE_TYPE type)
	: destination(destination), speed(speed), owner(owner), j1Entity(ENTITY_TYPE::PROJECTILE, pos.x, pos.y, name), type(type)
{
	debugSubtile = App->entityFactory->debugsubtileTex;
}

Projectile::~Projectile()
{
}


void Projectile::SetInitially()
{
	// Need to reposition it once we know the pivot so it takes direction properly
	position -= pivot;
	SetNewDirection(destination);
	angle = SetMyAngleRotation(direction);
}

bool Projectile::PostUpdate()
{
	if (TooFarAway()) {
		to_delete = true;
	}

	return true;
}

void Projectile::SetNewDirection(const fPoint & newdir)
{
	direction = newdir - GetPivotPos(); 
	direction.Normalize(); 
}

bool Projectile::OnCollisionWithEnemy()
{
	return (!App->entityFactory->isThisSubtileEnemyFree(GetSubtilePos()));
}

float Projectile::SetMyAngleRotation(const fPoint & direction)
{
	return RADTODEG * atan2f(direction.y, direction.x);
}

bool Projectile::TooFarAway() const
{
	// It checks its distance with the player, so if he moves towards it, the projectile won't delete
	return (App->entityFactory->player->GetPivotPos().DistanceManhattan(GetPivotPos()) > DESTRUCTIONRANGE);
}

bool Projectile::OnCollisionWithWall() const
{
	return (!App->pathfinding->IsWalkable(GetTilePos()));
}

bool Projectile::ReachedDestiny() const
{
	return (GetPivotPos().DistanceManhattan(destination) < 20);
}

void Projectile::Draw()
{
	iPoint subTilePos = GetSubtilePos();
	subTilePos = App->map->SubTileMapToWorld(subTilePos.x, subTilePos.y);
	App->render->Blit(debugSubtile, subTilePos.x, subTilePos.y, NULL);

	if (entityTex != nullptr)
	{
		if (currentAnimation != nullptr)
			App->render->Blit(entityTex, position.x, position.y, &currentAnimation->GetCurrentFrame(), 1.0F, SDL_FLIP_NONE, 1.0F, angle, pivot.x * App->win->GetScale(), pivot.y * App->win->GetScale());
		else
			App->render->Blit(entityTex, position.x, position.y, &drawAtlasRect, 1.0F, SDL_FLIP_NONE, 1.0F, angle, pivot.x, pivot.y);
	}
}



