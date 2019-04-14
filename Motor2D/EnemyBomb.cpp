#include "EnemyBomb.h"
#include "j1Textures.h"
#include "j1EntityFactory.h"
#include "j1Map.h"

EnemyBomb::EnemyBomb(iPoint position, uint speed, uint detectionRange, uint attackRange, uint baseDamage) : Enemy(position, speed, detectionRange, attackRange, baseDamage, 0)
{
	entityTex = App->tex->Load("textures/enemies/enemyBomb.png"); // TODO: note, repetead entities/enemies, load texture once time
	debugSubtile = App->tex->Load("maps/tile_32x32_2.png");



}
EnemyBomb::~EnemyBomb()
{
}
bool EnemyBomb::Start()
{
	return true;
}
bool EnemyBomb::PreUpdate()
{
	return true;
}
bool EnemyBomb::Update(float dt)
{
	SetState(dt); 
	return true;
}

bool EnemyBomb::PostUpdate()
{
	return true;
}

bool EnemyBomb::CleanUp()
{
	path_to_follow.clear();

	if (debugSubtile != nullptr)
	{
		App->tex->UnLoad(debugSubtile);
		debugSubtile = nullptr;
	}

	if (entityTex != nullptr)
	{
		App->tex->UnLoad(entityTex);
		entityTex = nullptr;
	}

	std::list<entityStat*>::iterator item = stat.begin();
	for (; item != stat.end(); ++item)
	{
		stat.remove(*item);
	}
	stat.clear();

	return true;
}

void EnemyBomb::SetState(float dt)
{
	switch (state)
	{
	case EnemyState::IDLE:
	{
		currentAnimation = &idle[pointingDir];
		if (isInDetectionRange() && speed > 0)
		{
			SetLookingTo(App->entityFactory->player->GetPivotPos());
			state = EnemyState::SEARCHPATH;
		}
		// else go back to spawn point?

	}
	break;
	case EnemyState::SEARCHPATH:
	{
		//freePass = false; 

		if (!isSubpathRange)
		{
			SearchNewPath();
			//LOG("Pathing");
		}
		else if (!App->entityFactory->areAllSubtilesReserved())
		{
			SearchNewSubPath();
			//LOG("Subpathing");
		}

		if (path_to_follow.size() > 0)
			state = EnemyState::GET_NEXT_TILE;

		else state = EnemyState::CHECK;
	}
	break;

	case EnemyState::GET_NEXT_TILE:
	{
		iPoint tileToGo = path_to_follow.front();
		if (!isSubpathRange)
		{
			currentDestiny = App->map->MapToWorld(tileToGo.x + 1, tileToGo.y);
			currentDestiny = { currentDestiny.x, currentDestiny.y + (int)(App->map->data.tile_height * 0.5F) }; // Center of the tile
		}
		else
		{
			currentDestiny = App->map->SubTileMapToWorld(tileToGo.x + 1, tileToGo.y);
			currentDestiny = { currentDestiny.x, currentDestiny.y + (int)(App->map->data.tile_height * 0.5F * 0.5F) };
		}
		state = EnemyState::GO_NEXT_TILE;
	}
	break;

	case EnemyState::GO_NEXT_TILE:
	{
		velocity = currentDestiny.Return_fPoint() - GetPivotPos();
		velocity.Normalize();
		position += velocity * dt * speed;

		//currentAnimation = &idle[(int)GetPointingDir(atan2f(velocity.y, velocity.x))];
		SetLookingTo(currentDestiny.Return_fPoint());
		currentAnimation = &run[pointingDir];

		//float current_cycle_frame = currentAnimation->GetCurrentFloatFrame();
		//currentAnimation->SetCurrentFrame(current_cycle_frame);

		state = EnemyState::CHECK;
	}
	break;

	case EnemyState::CHECK:
	{

		if (App->entityFactory->player->ChangedTile())
		{
			App->entityFactory->ReleaseAllReservedSubtiles();
			if (checkTime.Read() > GetRandomValue(250, 1000))
			{
				path_to_follow.clear();
				state = EnemyState::IDLE;
			}
		}
		else if (isOnDestiny())
		{
			if (path_to_follow.size() == 1 && isSubpathRange)
			{
				App->entityFactory->FreeAdjacent(App->map->WorldToSubtileMap(currentDestiny.x, currentDestiny.y));
			}

			if (path_to_follow.size() > 0)
			{
				path_to_follow.erase(path_to_follow.begin());
				state = EnemyState::GET_NEXT_TILE;
			}
		}
		else state = EnemyState::GO_NEXT_TILE;

		if (path_to_follow.size() <= 0)
		{
			if (isInAttackRange())
			{
				SetLookingTo(App->entityFactory->player->GetPivotPos());
				//currentAnimation = &idle[pointingDir];
				state = EnemyState::ATTACK;
				LOG("Attacking!");
				checkTime.Start();
			}
			else state = EnemyState::IDLE;
		}


		// Changes range conditions

		if (GetTilePos().DistanceManhattan(App->entityFactory->player->GetTilePos()) < 3)
		{
			if (!isSubpathRange)
				state = EnemyState::SEARCHPATH;
			isSubpathRange = true;
		}
		else
		{
			if (isSubpathRange)
				state = EnemyState::SEARCHPATH;
			isSubpathRange = false;
		}
		break;
	}

	case EnemyState::ATTACK:
	{
		currentAnimation = &basicAttack[pointingDir];
		if (checkTime.ReadSec() > explosionSecs && currentAnimation->Finished())
		{
			App->attackManager->AddPropagationAttack(this, GetSubtilePos(), propagationType::BFS, baseDamage, 6, 60);
			to_delete = true;

		}
	}
	break;

	default:
		break;
	}
}
