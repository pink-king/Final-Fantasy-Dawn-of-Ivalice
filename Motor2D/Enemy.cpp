#include "Enemy.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1EntityFactory.h"
#include "j1PathFinding.h"
#include "j1Map.h"
#include "j1LootManager.h"
#include <random>

Enemy::Enemy(iPoint position, uint movementSpeed, uint detectionRange, uint attackRange, uint baseDamage, float attackSpeed) 
	: speed(movementSpeed), detectionRange(detectionRange), baseDamage(baseDamage), attackRange(attackRange), j1Entity(ENEMY_TEST, position.x, position.y, "ENEMY_TEST")
{
	currentAnimation = &idle[(int)facingDirectionEnemy::SE];
	this->attackSpeed = 1.f / attackSpeed;
}

Enemy::~Enemy()
{
	// TODO: Loot spawn in all enemies? 
	App->attackManager->DestroyAllMyCurrentAttacks(this);
	LOG("parent enemy bye");
}

bool Enemy::SearchNewPath()
{
	bool ret = false;
	if (path_to_follow.size() > 0)
	{
		std::vector<iPoint>::iterator item = path_to_follow.begin();
		for (; item != path_to_follow.end(); ++item)
		{
			if (App->entityFactory->isThisSubtileReserved(*item))
			{
				App->entityFactory->FreeAdjacent(*item);
				break;
			}
		}
	}

	path_to_follow.clear();
	iPoint thisTile = App->map->WorldToMap((int)GetPivotPos().x, (int)GetPivotPos().y);
	iPoint playerTile = App->map->WorldToMap((int)App->entityFactory->player->GetPivotPos().x, (int)App->entityFactory->player->GetPivotPos().y);

	if (thisTile.DistanceManhattan(playerTile) > 1) // The enemy doesnt collapse with the player
	{
		if (App->pathfinding->CreatePath(thisTile, playerTile) > 0)
		{
			path_to_follow = *App->pathfinding->GetLastPath();
			if (path_to_follow.size() > 1)
				path_to_follow.erase(path_to_follow.begin());		// Enemy doesnt go to the center of his initial tile

			if (path_to_follow.size() > 1)
				path_to_follow.pop_back();							// Enemy doesnt eat the player, stays at 1 tile
				//path_to_follow.pop_back();
			ret = (path_to_follow.size() > 0);
		}
		//else LOG("Could not create path correctly");
	}

	return ret;
}

bool Enemy::SearchNewSubPath()
{
	bool ret = false;
	if (path_to_follow.size() > 0)
	{
		std::vector<iPoint>::iterator item = path_to_follow.begin();
		for (; item != path_to_follow.end(); ++item)
		{
			if (App->entityFactory->isThisSubtileReserved(*item))
			{
				App->entityFactory->FreeAdjacent(*item);
				break;
			}
		}
	}

	path_to_follow.clear();
	iPoint thisTile = App->map->WorldToSubtileMap((int)GetPivotPos().x, (int)GetPivotPos().y);
	iPoint playerTile = App->entityFactory->player->GetSubtilePos();

	if (thisTile.DistanceManhattan(playerTile) > 1) // The enemy doesnt collapse with the player
	{
		if (App->pathfinding->CreateSubtilePath(thisTile, playerTile) > 0)
		{
			path_to_follow = *App->pathfinding->GetLastPath();
			if (path_to_follow.size() > 1)
				path_to_follow.erase(path_to_follow.begin());		// Enemy doesnt go to the center of his initial tile

			if (path_to_follow.size() > 1)
				path_to_follow.pop_back();							// Enemy doesnt eat the player, stays at 1 tile

			iPoint adj = path_to_follow.back();
			App->entityFactory->ReserveAdjacent(adj);
			ret = (path_to_follow.size() > 0);
		}
		else LOG("Could not create path correctly");
	}

	return ret;
}

int Enemy::GetRandomValue(const int& min, const int& max) const
{
	static std::default_random_engine generator;
	std::uniform_int_distribution<int> distribution(min, max);
	float dice_roll = distribution(generator);
	return dice_roll;
}

bool Enemy::isInDetectionRange() const
{
	iPoint playerPos = App->entityFactory->player->GetTilePos(); 
	return (GetTilePos().DistanceManhattan(playerPos) < detectionRange);
}

bool Enemy::isInAttackRange() const
{
	return (GetSubtilePos().DistanceTo(App->entityFactory->player->GetSubtilePos()) <= attackRange);
}

bool Enemy::isNextPosFree(iPoint futurePos)
{
	iPoint onSubtilePosTemp = App->map->WorldToSubtileMap(futurePos.x, futurePos.y);

	return !(onSubtilePosTemp != previousSubtilePos && !App->entityFactory->isThisSubtileEnemyFree(onSubtilePosTemp));
}

bool Enemy::isOnDestiny() const
{
	return GetPivotPos().DistanceTo(currentDestiny.Return_fPoint()) < 5;
}


void Enemy::DebugPath() const
{
	for (uint i = 0; i < path_to_follow.size(); ++i)
	{
		if (!isSubpathRange) {
			iPoint pos = App->map->MapToWorld(path_to_follow[i].x + 1, path_to_follow[i].y);		// X + 1, Same problem with map
			App->render->DrawQuad({ pos.x, pos.y + (int)(App->map->data.tile_height * 0.5F), 5, 5 }, 255 , 0, 255, 175, true);
			//App->render->Blit(App->pathfinding->debug_texture, pos.x, pos.y);
		}
		else
		{
			iPoint pos = App->map->SubTileMapToWorld(path_to_follow[i].x + 1, path_to_follow[i].y);		// X + 1, Same problem with map
			App->render->DrawQuad({ pos.x, pos.y + (int)(App->map->data.tile_height * 0.5F * 0.5F), 3, 3 }, 255, 0, 255, 175, true);
			//App->render->Blit(App->pathfinding->debug_texture, pos.x, pos.y);
		}

	}

	iPoint subTilePos = GetSubtilePos();
	subTilePos = App->map->SubTileMapToWorld(subTilePos.x, subTilePos.y);
	App->render->Blit(debugSubtile, subTilePos.x, subTilePos.y, NULL);
}


void Enemy::Draw() 
{

	DebugPath();

	if (entityTex != nullptr)
	{
		if (currentAnimation != nullptr)
			App->render->Blit(entityTex, position.x, position.y, &currentAnimation->GetCurrentFrame(), 1.0F, flip);
		else
			App->render->Blit(entityTex, position.x, position.y);
	}	
}
