#include "Projectile.h"
#include "j1EntityFactory.h"
#include "j1PathFinding.h"
#include "j1Render.h"


Projectile::Projectile(fPoint pos, fPoint destination, uint speed, const j1Entity * owner, const char* name) 
	: destination(destination), speed(speed), owner(owner), j1Entity(ENTITY_TYPE::NO_TYPE, pos.x, pos.y, name) 
{
	debugSubtile = App->entityFactory->debugsubtileTex;

	direction = destination - GetPivotPos();
	direction.Normalize();
	angle = SetMyAngleRotation(direction);

}

Projectile::~Projectile()
{
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
			App->render->Blit(entityTex, position.x, position.y, &currentAnimation->GetCurrentFrame(), 1.0F, SDL_FLIP_NONE, 1.0F, angle, pivot.x * 2, pivot.y * 2);
		else
			App->render->Blit(entityTex, position.x, position.y, &drawAtlasRect, 1.0F, SDL_FLIP_NONE, 1.0F, angle, pivot.x, pivot.y);
	}
}



