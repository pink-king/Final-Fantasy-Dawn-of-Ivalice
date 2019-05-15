#include "EnemyArcher.h" 
#include "j1EntityFactory.h"


EnemyArcher::EnemyArcher(const iPoint & pos, bool dummy) : Enemy(pos, 70, 10, 7, 5, 2.F, dummy, ENTITY_TYPE::ENEMY_ARCHER, "EnemyArcher")
{
	LoadAnims(); 
}

EnemyArcher::~EnemyArcher()
{
}

bool EnemyArcher::PreUpdate()
{
	return true;
}

bool EnemyArcher::Update(float dt)
{
	SetState(dt); 
	return true;
}

bool EnemyArcher::PostUpdate()
{
	if (to_die)
		state = EnemyState::DYING;

	return true;
}

bool EnemyArcher::CleanUp()
{
	return true;
}

void EnemyArcher::SetState(float dt)
{
	switch (state)
	{
	case EnemyState::IDLE:
	{
		currentAnimation = &idle[pointingDir];
		if (isInDetectionRange() && !dummy)
		{
			/*App->audio->PlayFx(App->entityFactory->goblinDetection, 0);*/
			SetLookingTo(App->entityFactory->player->GetPivotPos());
			state = EnemyState::SEARCHPATH;
		}
		// else go back to spawn point?

	}
	break;
	case EnemyState::SEARCHPATH:
	{
		SearchNewPath();

		if (path_to_follow.size() > 0)
			state = EnemyState::GET_NEXT_TILE;

		else state = EnemyState::CHECK;
	}
	break;

	case EnemyState::GET_NEXT_TILE:
	{
		iPoint tileToGo = path_to_follow.front();
		
		currentDestiny = App->map->MapToWorld(tileToGo.x + 1, tileToGo.y);
		currentDestiny = { currentDestiny.x, currentDestiny.y + (int)(App->map->data.tile_height * 0.5F) }; // Center of the tile
	
		SetNewDirection();
		state = EnemyState::GO_NEXT_TILE;
	}
	break;

	case EnemyState::GO_NEXT_TILE:
	{
		if (CheckFuturePos(dt) || freePass)
		{
			MoveToCurrDestiny(dt);
			state = EnemyState::CHECK;
		}
		else
		{
			state = EnemyState::WAITING;
			checkTime.Start();
		}

	}
	break;

	case EnemyState::CHECK:
	{

		if (App->entityFactory->player->ChangedTile())		// Repath when player changes tiles (and random values)
		{
			App->entityFactory->ReleaseAllReservedSubtiles();
			if (checkTime.Read() > GetRandomValue(250, 1000))
			{
				path_to_follow.clear();
				state = EnemyState::IDLE;
			}
		}
		else if (isOnDestiny())							// Got to the center of the destiny tile
		{
			if (path_to_follow.size() == 1 && isSubpathRange)
			{
				App->entityFactory->FreeAdjacent(App->map->WorldToSubtileMap(currentDestiny.x, currentDestiny.y));
			}

			if (path_to_follow.size() > 0)
			{
				path_to_follow.erase(path_to_follow.begin());
				state = EnemyState::GET_NEXT_TILE;                // Keep going to the destiny tile
			}
		}
		else state = EnemyState::GO_NEXT_TILE;

		if (isInAttackRange())
		{
			path_to_follow.clear(); 
			SetLookingTo(App->entityFactory->player->GetPivotPos());
			//currentAnimation = &idle[pointingDir];
			state = EnemyState::ATTACK;
			currentAnimation = &basicAttack[pointingDir];
			LOG("Attacking!");
			checkTime.Start();
			break;
		}

		if (path_to_follow.empty())
			state = EnemyState::IDLE;

		break;

	}

	case EnemyState::ATTACK:
	{

		if (/*checkTime.ReadSec() > attackPerS &&*/ (int)currentAnimation->GetCurrentFloatFrame() == 2 && !attacked)
		{
			App->entityFactory->CreateArrow(GetThrowingPos(), App->entityFactory->player->GetPivotPos(), 100, this, PROJECTILE_TYPE::ENEMY_ARROW);
			attacked = true;
		}
		if (currentAnimation->Finished())
		{
			basicAttack[pointingDir].Reset();
			attacked = false;
			state = EnemyState::CHECK;
		}
	}
	break;

	case EnemyState::WAITING:	// Needs a re-planning
	{
		currentAnimation = &idle[pointingDir];
		static int cont = 0;
		if (checkTime.Read() > 2)
		{
			state = EnemyState::GO_NEXT_TILE;
			cont++;
		}
		if (cont > 30)
		{
			freePass = true;
			cont = 0;
			LOG("Gave a free pass!");
		}
		break;
	}
	case EnemyState::DYING:
		currentAnimation = &dyingAnim;
		if (currentAnimation->Finished()) {
			to_delete = true;
		}
		break;
	default:
		break;
	}
}

void EnemyArcher::LoadAnims()
{
	entityTex = App->entityFactory->enemyZombieTex;

	// TODO: import from xml

	idle[(int)facingDirectionEnemy::SE].PushBack({ 38, 8, 18, 30 });
	idle[(int)facingDirectionEnemy::S].PushBack({ 116, 8, 15, 30 });
	idle[(int)facingDirectionEnemy::SW].PushBack({ 38, 8, 18, 30 });
	idle[(int)facingDirectionEnemy::NE].PushBack({ 191, 8, 18, 30 });
	idle[(int)facingDirectionEnemy::NW].PushBack({ 191, 8, 18, 30 });
	idle[(int)facingDirectionEnemy::N].PushBack({ 269, 8, 14, 30 });
	idle[(int)facingDirectionEnemy::W].PushBack({ 250, 49, 16, 30 });
	idle[(int)facingDirectionEnemy::E].PushBack({ 250, 49, 16, 30 });

	float animSpeed = 5.5F;

	run[(int)facingDirectionEnemy::SE].PushBack({ 10, 9, 19, 30 });
	run[(int)facingDirectionEnemy::SE].PushBack({ 38, 8, 18, 30 });
	run[(int)facingDirectionEnemy::SE].PushBack({ 66, 9, 18, 29 });
	run[(int)facingDirectionEnemy::SE].PushBack({ 38, 8, 18, 30 });
	run[(int)facingDirectionEnemy::SE].speed = animSpeed;

	run[(int)facingDirectionEnemy::SW].PushBack({ 10, 9, 19, 30 });
	run[(int)facingDirectionEnemy::SW].PushBack({ 38, 8, 18, 30 });
	run[(int)facingDirectionEnemy::SW].PushBack({ 66, 9, 18, 29 });
	run[(int)facingDirectionEnemy::SW].PushBack({ 38, 8, 18, 30 });
	run[(int)facingDirectionEnemy::SW].speed = animSpeed;

	run[(int)facingDirectionEnemy::S].PushBack({ 97, 9, 15, 30 });
	run[(int)facingDirectionEnemy::S].PushBack({ 116, 8, 15, 30 });
	run[(int)facingDirectionEnemy::S].PushBack({ 136, 10, 14, 29 });
	run[(int)facingDirectionEnemy::S].PushBack({ 116, 8, 15, 30 });
	run[(int)facingDirectionEnemy::S].speed = animSpeed;


	run[(int)facingDirectionEnemy::NE].PushBack({ 162, 9, 18, 29 });
	run[(int)facingDirectionEnemy::NE].PushBack({ 191, 8, 18, 30 });
	run[(int)facingDirectionEnemy::NE].PushBack({ 221, 9, 17, 30 });
	run[(int)facingDirectionEnemy::NE].PushBack({ 191, 8, 18, 30 });
	run[(int)facingDirectionEnemy::NE].speed = animSpeed;

	run[(int)facingDirectionEnemy::NW].PushBack({ 162, 9, 18, 29 });
	run[(int)facingDirectionEnemy::NW].PushBack({ 191, 8, 18, 30 });
	run[(int)facingDirectionEnemy::NW].PushBack({ 221, 9, 17, 30 });
	run[(int)facingDirectionEnemy::NW].PushBack({ 191, 8, 18, 30 });
	run[(int)facingDirectionEnemy::NW].speed = animSpeed;

	run[(int)facingDirectionEnemy::N].PushBack({ 250, 9, 14, 30 });
	run[(int)facingDirectionEnemy::N].PushBack({ 269, 8, 14, 30 });
	run[(int)facingDirectionEnemy::N].PushBack({ 288, 9, 14, 30 });
	run[(int)facingDirectionEnemy::N].PushBack({ 269, 8, 14, 30 });
	run[(int)facingDirectionEnemy::N].speed = animSpeed;

	run[(int)facingDirectionEnemy::E].PushBack({ 222, 51, 21, 27 });
	run[(int)facingDirectionEnemy::E].PushBack({ 250, 49, 16, 30 });
	run[(int)facingDirectionEnemy::E].PushBack({ 268, 51, 21, 27 });
	run[(int)facingDirectionEnemy::E].PushBack({ 250, 49, 16, 30 });
	run[(int)facingDirectionEnemy::E].speed = animSpeed;

	run[(int)facingDirectionEnemy::W].PushBack({ 222, 51, 21, 27 });
	run[(int)facingDirectionEnemy::W].PushBack({ 250, 49, 16, 30 });
	run[(int)facingDirectionEnemy::W].PushBack({ 268, 51, 21, 27 });
	run[(int)facingDirectionEnemy::W].PushBack({ 250, 49, 16, 30 });
	run[(int)facingDirectionEnemy::W].speed = animSpeed;

	float attackSpeedAnim = attackSpeed * 4.F;

	basicAttack[(int)facingDirectionEnemy::SE].PushBack({ 14, 89, 15, 29 });
	basicAttack[(int)facingDirectionEnemy::SE].PushBack({ 36, 87, 18, 31 });
	basicAttack[(int)facingDirectionEnemy::SE].PushBack({ 64, 92, 21, 26 });
	basicAttack[(int)facingDirectionEnemy::SE].PushBack({ 97, 90, 20, 28 });
	basicAttack[(int)facingDirectionEnemy::SE].loop = false;
	basicAttack[(int)facingDirectionEnemy::SE].speed = attackSpeedAnim;

	basicAttack[(int)facingDirectionEnemy::SW].PushBack({ 14, 89, 15, 29 });
	basicAttack[(int)facingDirectionEnemy::SW].PushBack({ 36, 87, 18, 31 });
	basicAttack[(int)facingDirectionEnemy::SW].PushBack({ 64, 92, 21, 26 });
	basicAttack[(int)facingDirectionEnemy::SW].PushBack({ 97, 90, 20, 28 });
	basicAttack[(int)facingDirectionEnemy::SW].loop = false;
	basicAttack[(int)facingDirectionEnemy::SW].speed = attackSpeedAnim;

	basicAttack[(int)facingDirectionEnemy::S].PushBack({ 135, 90, 18, 28 });
	basicAttack[(int)facingDirectionEnemy::S].PushBack({ 162, 87, 21, 32 });
	basicAttack[(int)facingDirectionEnemy::S].PushBack({ 189, 92, 14, 27 });
	basicAttack[(int)facingDirectionEnemy::S].PushBack({ 208, 91, 16, 28 });
	basicAttack[(int)facingDirectionEnemy::S].loop = false;
	basicAttack[(int)facingDirectionEnemy::S].speed = attackSpeedAnim;

	basicAttack[(int)facingDirectionEnemy::N].PushBack({ 351, 91, 18, 27 });
	basicAttack[(int)facingDirectionEnemy::N].PushBack({ 379, 87, 18, 31 });
	basicAttack[(int)facingDirectionEnemy::N].PushBack({ 405, 91, 14, 27 });
	basicAttack[(int)facingDirectionEnemy::N].PushBack({ 425, 90, 14, 28 });
	basicAttack[(int)facingDirectionEnemy::N].loop = false;
	basicAttack[(int)facingDirectionEnemy::N].speed = attackSpeedAnim;

	basicAttack[(int)facingDirectionEnemy::NE].PushBack({ 234, 90, 15, 28 });
	basicAttack[(int)facingDirectionEnemy::NE].PushBack({ 265, 88, 18, 30 });
	basicAttack[(int)facingDirectionEnemy::NE].PushBack({ 293, 91, 18, 27 });
	basicAttack[(int)facingDirectionEnemy::NE].PushBack({ 321, 90, 19, 28 });
	basicAttack[(int)facingDirectionEnemy::NE].speed = attackSpeedAnim;
	basicAttack[(int)facingDirectionEnemy::NE].loop = false;


	basicAttack[(int)facingDirectionEnemy::NW].PushBack({ 234, 90, 15, 28 });
	basicAttack[(int)facingDirectionEnemy::NW].PushBack({ 265, 88, 18, 30 });
	basicAttack[(int)facingDirectionEnemy::NW].PushBack({ 293, 91, 18, 27 });
	basicAttack[(int)facingDirectionEnemy::NW].PushBack({ 321, 90, 19, 28 });
	basicAttack[(int)facingDirectionEnemy::NW].speed = attackSpeedAnim;
	basicAttack[(int)facingDirectionEnemy::NW].loop = false;

	basicAttack[(int)facingDirectionEnemy::E].PushBack({ 331, 54, 19, 27 });
	basicAttack[(int)facingDirectionEnemy::E].PushBack({ 351, 50, 22, 32 });
	basicAttack[(int)facingDirectionEnemy::E].PushBack({ 375, 55, 18, 27 });
	basicAttack[(int)facingDirectionEnemy::E].PushBack({ 395, 54, 19, 28 });
	basicAttack[(int)facingDirectionEnemy::E].speed = attackSpeedAnim;
	basicAttack[(int)facingDirectionEnemy::E].loop = false;

	basicAttack[(int)facingDirectionEnemy::W].PushBack({ 331, 54, 19, 27 });
	basicAttack[(int)facingDirectionEnemy::W].PushBack({ 351, 50, 22, 32 });
	basicAttack[(int)facingDirectionEnemy::W].PushBack({ 375, 55, 18, 27 });
	basicAttack[(int)facingDirectionEnemy::W].PushBack({ 395, 54, 19, 28 });
	basicAttack[(int)facingDirectionEnemy::W].speed = attackSpeedAnim;
	basicAttack[(int)facingDirectionEnemy::W].loop = false;

	dyingAnim.PushBack({ 12, 129, 18, 29 });
	dyingAnim.PushBack({ 40, 129, 18, 29 });
	dyingAnim.PushBack({ 68, 130, 18, 29 });
	dyingAnim.PushBack({ 94, 130, 19, 29 });
	dyingAnim.loop = false;
	dyingAnim.speed = 5.F;

	//currentAnimation = &run[(int)facingDirectionEnemy::SE];
	/*SetPivot(8, 32);
	size.create(13,36);*/

	SetPivot(8, 26);
	size.create(18, 29);
	this->position -= pivot;
}
