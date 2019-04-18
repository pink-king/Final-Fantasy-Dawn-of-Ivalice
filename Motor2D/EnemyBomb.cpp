#include "EnemyBomb.h"
#include "j1Textures.h"
#include "j1EntityFactory.h"
#include "j1Map.h"

EnemyBomb::EnemyBomb(iPoint position, uint speed, uint detectionRange, uint attackRange, uint baseDamage) : Enemy(position, speed, detectionRange, attackRange, baseDamage, 0, ENTITY_TYPE::ENEMY_BOMB, "Enemy Bomb")
{
	// TODO: Load From XML
	entityTex = App->entityFactory->enemyBombTex; 
	
	idle[(int)facingDirectionEnemy::SE].PushBack({ 20, 2, 15, 22 });
	idle[(int)facingDirectionEnemy::S].PushBack({ 19, 27, 16, 22 });
	idle[(int)facingDirectionEnemy::SW].PushBack({ 20, 2, 15, 22 });
	idle[(int)facingDirectionEnemy::NE].PushBack({ 19, 52, 15, 21 });
	idle[(int)facingDirectionEnemy::NW].PushBack({ 19, 52, 15, 21 });
	idle[(int)facingDirectionEnemy::N].PushBack({ 19, 77, 16, 21 });
	idle[(int)facingDirectionEnemy::W].PushBack({ 20, 101, 19, 21 });
	idle[(int)facingDirectionEnemy::E].PushBack({ 20, 101, 19, 21 });


	run[(int)facingDirectionEnemy::SE].PushBack({ 3, 3, 15, 20 });
	run[(int)facingDirectionEnemy::SE].PushBack({ 20, 2, 15, 22 });
	run[(int)facingDirectionEnemy::SE].PushBack({ 37, 4, 15, 21 });
	run[(int)facingDirectionEnemy::SE].PushBack({ 20, 2, 15, 22 });
	run[(int)facingDirectionEnemy::SE].speed = 7.F;

	run[(int)facingDirectionEnemy::SW].PushBack({ 3, 3, 15, 20 });
	run[(int)facingDirectionEnemy::SW].PushBack({ 20, 2, 15, 22 });
	run[(int)facingDirectionEnemy::SW].PushBack({ 37, 4, 15, 21 });
	run[(int)facingDirectionEnemy::SW].PushBack({ 20, 2, 15, 22 });
	run[(int)facingDirectionEnemy::SW].speed = 7.F;

	run[(int)facingDirectionEnemy::S].PushBack({ 2, 28, 16, 20 });
	run[(int)facingDirectionEnemy::S].PushBack({ 19, 27, 16, 22 });
	run[(int)facingDirectionEnemy::S].PushBack({ 36, 28, 16, 22 });
	run[(int)facingDirectionEnemy::S].PushBack({ 19, 27, 16, 22 });
	run[(int)facingDirectionEnemy::S].speed = 7.F;


	run[(int)facingDirectionEnemy::NE].PushBack({ 2, 54, 15, 18 });
	run[(int)facingDirectionEnemy::NE].PushBack({ 19, 52, 15, 21 });
	run[(int)facingDirectionEnemy::NE].PushBack({ 35, 53, 15, 21 });
	run[(int)facingDirectionEnemy::NE].PushBack({ 19, 52, 15, 21 });
	run[(int)facingDirectionEnemy::NE].speed = 7.F;

	run[(int)facingDirectionEnemy::NW].PushBack({ 2, 54, 15, 18 });
	run[(int)facingDirectionEnemy::NW].PushBack({ 19, 52, 15, 21 });
	run[(int)facingDirectionEnemy::NW].PushBack({ 35, 53, 15, 21 });
	run[(int)facingDirectionEnemy::NW].PushBack({ 19, 52, 15, 21 });
	run[(int)facingDirectionEnemy::NW].speed = 7.F;

	run[(int)facingDirectionEnemy::N].PushBack({ 2, 79, 16, 18 });
	run[(int)facingDirectionEnemy::N].PushBack({ 19, 77, 16, 21 });
	run[(int)facingDirectionEnemy::N].PushBack({ 36, 78, 16, 21 });
	run[(int)facingDirectionEnemy::N].PushBack({ 19, 77, 16, 21 });
	run[(int)facingDirectionEnemy::N].speed = 7.F;

	run[(int)facingDirectionEnemy::E].PushBack({ 2, 102, 17, 19 });
	run[(int)facingDirectionEnemy::E].PushBack({ 20, 101, 19, 21});
	run[(int)facingDirectionEnemy::E].PushBack({ 39, 102, 19, 21 });
	run[(int)facingDirectionEnemy::E].PushBack({ 20, 101, 19, 21 });
	run[(int)facingDirectionEnemy::E].speed = 7.F;

	run[(int)facingDirectionEnemy::W].PushBack({ 2, 102, 17, 19 });
	run[(int)facingDirectionEnemy::W].PushBack({ 20, 101, 19, 21 });
	run[(int)facingDirectionEnemy::W].PushBack({ 39, 102, 19, 21 });
	run[(int)facingDirectionEnemy::W].PushBack({ 20, 101, 19, 21 });
	run[(int)facingDirectionEnemy::W].speed = 7.F;


	basicAttack[(int)facingDirectionEnemy::SW].PushBack({ 57, 2, 14, 22 });
	basicAttack[(int)facingDirectionEnemy::SW].PushBack({ 73, 3, 14, 21});
	basicAttack[(int)facingDirectionEnemy::SW].PushBack({ 91, 2, 16, 17});
	basicAttack[(int)facingDirectionEnemy::SW].PushBack({ 108, 2, 15, 18});
	basicAttack[(int)facingDirectionEnemy::SW].loop = false;
	basicAttack[(int)facingDirectionEnemy::SW].speed = 8.F;

	basicAttack[(int)facingDirectionEnemy::SE].PushBack({ 57, 2, 14, 22 });
	basicAttack[(int)facingDirectionEnemy::SE].PushBack({ 73, 3, 14, 21 });
	basicAttack[(int)facingDirectionEnemy::SE].PushBack({ 91, 2, 16, 17 });
	basicAttack[(int)facingDirectionEnemy::SE].PushBack({ 108, 2, 15, 18 });
	basicAttack[(int)facingDirectionEnemy::SE].loop = false;
	basicAttack[(int)facingDirectionEnemy::SE].speed = 8.F;

	basicAttack[(int)facingDirectionEnemy::S].PushBack({ 57, 27, 16, 20});
	basicAttack[(int)facingDirectionEnemy::S].PushBack({ 74, 27, 16, 21 });
	basicAttack[(int)facingDirectionEnemy::S].PushBack({ 91, 26, 16, 20});
	basicAttack[(int)facingDirectionEnemy::S].PushBack({ 108, 26, 16, 19});
	basicAttack[(int)facingDirectionEnemy::S].loop = false;
	basicAttack[(int)facingDirectionEnemy::S].speed = 8.F;

	basicAttack[(int)facingDirectionEnemy::N].PushBack({ 57, 78, 16, 18 });
	basicAttack[(int)facingDirectionEnemy::N].PushBack({ 74, 76, 16, 21 });
	basicAttack[(int)facingDirectionEnemy::N].PushBack({ 91, 76, 16, 21 });
	basicAttack[(int)facingDirectionEnemy::N].PushBack({ 108, 77, 16, 19 });
	basicAttack[(int)facingDirectionEnemy::N].loop = false;
	basicAttack[(int)facingDirectionEnemy::N].speed = 8.F;

	basicAttack[(int)facingDirectionEnemy::NE].PushBack({ 57, 52, 15, 19});
	basicAttack[(int)facingDirectionEnemy::NE].PushBack({ 74, 51, 16, 21 });
	basicAttack[(int)facingDirectionEnemy::NE].PushBack({ 91, 56, 16, 19 });
	basicAttack[(int)facingDirectionEnemy::NE].PushBack({ 110, 56, 14, 18});
	basicAttack[(int)facingDirectionEnemy::NE].loop = false;
	basicAttack[(int)facingDirectionEnemy::NE].speed = 8.F;

	basicAttack[(int)facingDirectionEnemy::NW].PushBack({ 57, 52, 15, 19 });
	basicAttack[(int)facingDirectionEnemy::NW].PushBack({ 74, 51, 16, 21 });
	basicAttack[(int)facingDirectionEnemy::NW].PushBack({ 91, 56, 16, 19 });
	basicAttack[(int)facingDirectionEnemy::NW].PushBack({ 110, 56, 14, 18 });
	basicAttack[(int)facingDirectionEnemy::NW].loop = false;
	basicAttack[(int)facingDirectionEnemy::NW].speed = 8.F;


	basicAttack[(int)facingDirectionEnemy::E].PushBack({ 63, 101, 15, 19 });
	basicAttack[(int)facingDirectionEnemy::E].PushBack({ 80, 101, 18, 20});
	basicAttack[(int)facingDirectionEnemy::E].PushBack({ 102, 103, 15, 16});
	basicAttack[(int)facingDirectionEnemy::E].PushBack({ 119, 102, 15, 16});
	basicAttack[(int)facingDirectionEnemy::E].loop = false;
	basicAttack[(int)facingDirectionEnemy::E].speed = 8.F;

	basicAttack[(int)facingDirectionEnemy::W].PushBack({ 63, 101, 15, 19 });
	basicAttack[(int)facingDirectionEnemy::W].PushBack({ 80, 101, 18, 20 });
	basicAttack[(int)facingDirectionEnemy::W].PushBack({ 102, 103, 15, 16 });
	basicAttack[(int)facingDirectionEnemy::W].PushBack({ 119, 102, 15, 16 });
	basicAttack[(int)facingDirectionEnemy::W].loop = false;
	basicAttack[(int)facingDirectionEnemy::W].speed = 8.F;


	SetPivot(8, 18);
	size.create(16, 20);
	this->position -= pivot;
}
EnemyBomb::~EnemyBomb()
{
	App->attackManager->AddPropagationAttack(this, GetSubtilePos(), propagationType::BFS, baseDamage, 6, 60);
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

	

	std::list<entityStat*>::iterator item = stat.begin();
	for (; item != stat.end(); ++item)
	{
		stat.remove(*item);
	}
	stat.clear();


	// Textures Loaded and Unloaded in Entity Factory
	/*if (debugSubtile != nullptr)
	{
		App->tex->UnLoad(debugSubtile);
		debugSubtile = nullptr;
	}*/

	/*if (entityTex != nullptr)
	{
		App->tex->UnLoad(entityTex);
		entityTex = nullptr;
	}*/

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
			SearchNewSubPath(true);
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

		SetLookingTo(currentDestiny.Return_fPoint());
		currentAnimation = &run[pointingDir];

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

		if (checkTime.ReadSec() > explosionDelay && currentAnimation->Finished())
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
