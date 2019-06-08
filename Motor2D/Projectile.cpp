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
	// It belongs to the factory
	/*if (debugSubtile != nullptr)
	{
		App->tex->UnLoad(debugSubtile);
		debugSubtile = nullptr;
	}*/
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

float Projectile::SetMyAngleRotation(const fPoint & direction)
{
	return RADTODEG * atan2f(direction.y, direction.x);
}

bool Projectile::TooFarAway() const
{
	// It checks its distance with the player, so if he moves towards it, the projectile won't delete
	return (App->entityFactory->player->GetPivotPos().DistanceManhattan(GetPivotPos()) > DESTRUCTIONRANGE);
}

bool Projectile::OnCollisionWithEnemy(const int & radius) const
{
	if(radius == 0)
		return (!App->entityFactory->isThisSubtileEnemyFree(GetSubtilePos()));

	for (int i = -radius; i <= radius; ++i)
	{
		for (int j = -radius; j <= radius; ++j)
		{
			iPoint checkPos = GetSubtilePos() + iPoint(i, j);
			if (!App->entityFactory->isThisSubtileEnemyFree(checkPos))
				return true; 
		}
	}
	return false;
}

bool Projectile::OnCollisionWithWall(const int & radius) const
{
	if(radius == 0)
		return (!App->pathfinding->IsWalkable(GetTilePos()));

	for (int i = -radius; i <= radius; ++i)
	{
		for (int j = -radius; j <= radius; ++j)
		{
			iPoint mySubtile = GetSubtilePos() + iPoint(i, j);
			iPoint myTilePos = App->map->SubTileMapToWorld(mySubtile.x, mySubtile.y);
			if (!App->pathfinding->IsWalkable(App->map->WorldToMap(myTilePos.x + 8, myTilePos.y + 8)))
				return true; 	
		}
	}
	return false;
}

bool Projectile::OnCollisionWithPlayer(const int & radius) const
{
	if(radius == 0)
		return (!App->entityFactory->isThisSubtilePlayerFree(GetSubtilePos()));

	for (int i = -radius; i <= radius; ++i)
	{
		for (int j = -radius; j <= radius; ++j)
		{
			if (!App->entityFactory->isThisSubtilePlayerFree(GetSubtilePos() + iPoint(i, j)))
				return true;
		}
	}
	return false;
}

bool Projectile::ReachedDestiny() const
{
	return (GetPivotPos().DistanceManhattan(destination) < 20);
}

void Projectile::Draw()
{
	if (App->scene->debugSubtiles == true)
	{
	iPoint subTilePos = GetSubtilePos();
	subTilePos = App->map->SubTileMapToWorld(subTilePos.x, subTilePos.y);
	App->render->Blit(debugSubtile, subTilePos.x, subTilePos.y, NULL);
	}

	if (entityTex != nullptr)
	{
		if (currentAnimation != nullptr)
			App->render->Blit(entityTex, position.x, position.y, &currentAnimation->GetCurrentFrame(), 1.0F, SDL_FLIP_NONE, 1.0F, angle, pivot.x * App->win->GetScale(), pivot.y * App->win->GetScale());
		else
			App->render->Blit(entityTex, position.x, position.y, &drawAtlasRect, 1.0F, SDL_FLIP_NONE, 1.0F, angle, pivot.x, pivot.y);
	}
}

bool Projectile::Load(pugi::xml_node &)
{
	return true;
}

bool Projectile::Save(pugi::xml_node &) const
{
	return true;
}



const j1Entity* Projectile::GetOwnerEntity() const
{
	return owner;
}