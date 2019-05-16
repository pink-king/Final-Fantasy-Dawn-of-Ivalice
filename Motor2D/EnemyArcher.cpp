#include "EnemyArcher.h" 
#include "j1EntityFactory.h"
#include "j1BuffManager.h"

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
	if (!isParalize)
	{
		SetState(dt); 
	}

	if (stat.size() != 0)
	{
		if (App->buff->DamageInTime(this))
		{
			App->buff->entitiesTimeDamage.remove(this);
		}
	}

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
	entityTex = App->entityFactory->enemyGolemTex;

	// TODO: import from xml

	idle[(int)facingDirectionEnemy::SE].PushBack({ 64, 921, 64, 64 }); //
	idle[(int)facingDirectionEnemy::S].PushBack({ 64, 857, 64, 64 }); //
	idle[(int)facingDirectionEnemy::SW].PushBack({ 64, 921, 64, 64 }); //
	idle[(int)facingDirectionEnemy::NE].PushBack({ 64, 1049, 64, 64 }); //
	idle[(int)facingDirectionEnemy::NW].PushBack({ 64, 1049, 64, 64 }); //
	idle[(int)facingDirectionEnemy::N].PushBack({ 64, 1113, 64, 64 }); //
	idle[(int)facingDirectionEnemy::W].PushBack({ 64, 985, 64, 64 });  //
	idle[(int)facingDirectionEnemy::E].PushBack({ 64, 985, 64, 64 });  //

	float animSpeed = 5.5F;

	run[(int)facingDirectionEnemy::SE].PushBack({ 0, 921, 64, 64 }); //
	run[(int)facingDirectionEnemy::SE].PushBack({ 64, 921, 64, 64 });
	run[(int)facingDirectionEnemy::SE].PushBack({ 128, 921, 64, 64 });
	run[(int)facingDirectionEnemy::SE].PushBack({ 64, 921, 64, 64 });
	run[(int)facingDirectionEnemy::SE].speed = animSpeed;

	run[(int)facingDirectionEnemy::SW].PushBack({ 0, 921, 64, 64 }); //
	run[(int)facingDirectionEnemy::SW].PushBack({ 64, 921, 64, 64 });
	run[(int)facingDirectionEnemy::SW].PushBack({ 128, 921, 64, 64 });
	run[(int)facingDirectionEnemy::SW].PushBack({ 64, 921, 64, 64 });
	run[(int)facingDirectionEnemy::SW].speed = animSpeed;

	run[(int)facingDirectionEnemy::S].PushBack({ 0, 857, 64, 64 }); //
	run[(int)facingDirectionEnemy::S].PushBack({ 64, 857, 64, 64 });
	run[(int)facingDirectionEnemy::S].PushBack({ 128, 857, 64, 64 });
	run[(int)facingDirectionEnemy::S].PushBack({ 64, 857, 64, 64 });
	run[(int)facingDirectionEnemy::S].speed = animSpeed;


	run[(int)facingDirectionEnemy::NE].PushBack({ 0, 1049, 64, 64}); //
	run[(int)facingDirectionEnemy::NE].PushBack({ 64, 1049, 64, 64});
	run[(int)facingDirectionEnemy::NE].PushBack({ 128, 1049, 64, 64});
	run[(int)facingDirectionEnemy::NE].PushBack({ 64, 1049, 64, 64 });
	run[(int)facingDirectionEnemy::NE].speed = animSpeed;

	run[(int)facingDirectionEnemy::NW].PushBack({ 0, 1049, 64, 64 }); //
	run[(int)facingDirectionEnemy::NW].PushBack({ 64, 1049, 64, 64 });
	run[(int)facingDirectionEnemy::NW].PushBack({ 128, 1049, 64, 64 });
	run[(int)facingDirectionEnemy::NW].PushBack({ 64, 1049, 64, 64 });
	run[(int)facingDirectionEnemy::NW].speed = animSpeed;

	run[(int)facingDirectionEnemy::N].PushBack({ 0, 1113, 64, 64}); //
	run[(int)facingDirectionEnemy::N].PushBack({ 64, 1113, 64, 64});
	run[(int)facingDirectionEnemy::N].PushBack({ 128, 1113, 64, 64 });
	run[(int)facingDirectionEnemy::N].PushBack({ 64, 1113, 64, 64 });
	run[(int)facingDirectionEnemy::N].speed = animSpeed;

	run[(int)facingDirectionEnemy::E].PushBack({ 0, 985, 64, 64 }); //
	run[(int)facingDirectionEnemy::E].PushBack({ 64, 985, 64, 64 });
	run[(int)facingDirectionEnemy::E].PushBack({ 128, 985, 64, 64 });
	run[(int)facingDirectionEnemy::E].PushBack({ 64, 985, 64, 64 });
	run[(int)facingDirectionEnemy::E].speed = animSpeed;

	run[(int)facingDirectionEnemy::W].PushBack({ 0, 985, 64, 64 }); //
	run[(int)facingDirectionEnemy::W].PushBack({ 64, 985, 64, 64 });
	run[(int)facingDirectionEnemy::W].PushBack({ 128, 985, 64, 64 });
	run[(int)facingDirectionEnemy::W].PushBack({ 64, 985, 64, 64 });
	run[(int)facingDirectionEnemy::W].speed = animSpeed;

	float attackSpeedAnim = attackSpeed * 4.F;

	basicAttack[(int)facingDirectionEnemy::SE].PushBack({ 0, 569, 64, 64 }); //
	basicAttack[(int)facingDirectionEnemy::SE].PushBack({ 64, 569, 64, 64 });
	basicAttack[(int)facingDirectionEnemy::SE].PushBack({ 128, 569, 64, 64 });
	basicAttack[(int)facingDirectionEnemy::SE].PushBack({ 192, 569, 64, 64 });
	basicAttack[(int)facingDirectionEnemy::SE].loop = false;
	basicAttack[(int)facingDirectionEnemy::SE].speed = attackSpeedAnim;

	basicAttack[(int)facingDirectionEnemy::SW].PushBack({ 0, 569, 64, 64 }); //
	basicAttack[(int)facingDirectionEnemy::SW].PushBack({ 64, 569, 64, 64 });
	basicAttack[(int)facingDirectionEnemy::SW].PushBack({ 128, 569, 64, 64 });
	basicAttack[(int)facingDirectionEnemy::SW].PushBack({ 192, 569, 64, 64 });
	basicAttack[(int)facingDirectionEnemy::SW].loop = false;
	basicAttack[(int)facingDirectionEnemy::SW].speed = attackSpeedAnim;

	basicAttack[(int)facingDirectionEnemy::S].PushBack({ 0, 505, 64, 64}); //
	basicAttack[(int)facingDirectionEnemy::S].PushBack({ 64, 505, 64, 64 });
	basicAttack[(int)facingDirectionEnemy::S].PushBack({ 128, 505, 64, 64 });
	basicAttack[(int)facingDirectionEnemy::S].PushBack({ 192, 505, 64, 64 });
	basicAttack[(int)facingDirectionEnemy::S].loop = false;
	basicAttack[(int)facingDirectionEnemy::S].speed = attackSpeedAnim;

	basicAttack[(int)facingDirectionEnemy::N].PushBack({ 0, 761, 64, 64 }); //
	basicAttack[(int)facingDirectionEnemy::N].PushBack({ 64, 761, 64, 64 });
	basicAttack[(int)facingDirectionEnemy::N].PushBack({ 128, 761, 64, 64 });
	basicAttack[(int)facingDirectionEnemy::N].PushBack({ 192, 761, 64, 64 });
	basicAttack[(int)facingDirectionEnemy::N].loop = false;
	basicAttack[(int)facingDirectionEnemy::N].speed = attackSpeedAnim;

	basicAttack[(int)facingDirectionEnemy::NE].PushBack({ 0, 697, 64, 64 }); //
	basicAttack[(int)facingDirectionEnemy::NE].PushBack({ 64, 697, 64, 64 });
	basicAttack[(int)facingDirectionEnemy::NE].PushBack({ 128, 697, 64, 64 });
	basicAttack[(int)facingDirectionEnemy::NE].PushBack({ 192, 697, 64, 64 });
	basicAttack[(int)facingDirectionEnemy::NE].loop = false;
	basicAttack[(int)facingDirectionEnemy::NE].speed = attackSpeedAnim;


	basicAttack[(int)facingDirectionEnemy::NW].PushBack({ 0, 697, 64, 64 }); //
	basicAttack[(int)facingDirectionEnemy::NW].PushBack({ 64, 697, 64, 64 });
	basicAttack[(int)facingDirectionEnemy::NW].PushBack({ 128, 697, 64, 64 });
	basicAttack[(int)facingDirectionEnemy::NW].PushBack({ 192, 697, 64, 64 });
	basicAttack[(int)facingDirectionEnemy::NW].loop = false;
	basicAttack[(int)facingDirectionEnemy::NW].speed = attackSpeedAnim;

	basicAttack[(int)facingDirectionEnemy::E].PushBack({ 0, 633, 64, 64 }); //
	basicAttack[(int)facingDirectionEnemy::E].PushBack({ 64, 633, 64, 64 });
	basicAttack[(int)facingDirectionEnemy::E].PushBack({ 128, 633, 64, 64 });
	basicAttack[(int)facingDirectionEnemy::E].PushBack({ 192, 633, 64, 64 });
	basicAttack[(int)facingDirectionEnemy::E].loop = false;
	basicAttack[(int)facingDirectionEnemy::E].speed = attackSpeedAnim;

	basicAttack[(int)facingDirectionEnemy::W].PushBack({ 0, 633, 64, 64 }); //
	basicAttack[(int)facingDirectionEnemy::W].PushBack({ 64, 633, 64, 64 });
	basicAttack[(int)facingDirectionEnemy::W].PushBack({ 128, 633, 64, 64 });
	basicAttack[(int)facingDirectionEnemy::W].PushBack({ 192, 633, 64, 64 });
	basicAttack[(int)facingDirectionEnemy::W].loop = false;
	basicAttack[(int)facingDirectionEnemy::W].speed = attackSpeedAnim;



	// Hardcode mode 
	dyingAnim.PushBack({ 0, 313, 64, 64 }); 
	dyingAnim.PushBack({ 0, 313, 64, 64 }); 
	dyingAnim.PushBack({ 0, 313, 64, 64 }); 
	dyingAnim.PushBack({ 0, 313, 64, 64 }); 
	dyingAnim.PushBack({ 0, 441, 64, 64 }); 
	dyingAnim.PushBack({ 0, 441, 64, 64 }); 
	dyingAnim.PushBack({ 0, 441, 64, 64 }); 
	dyingAnim.PushBack({ 0, 441, 64, 64 }); 
	dyingAnim.loop = false;
	dyingAnim.speed = 5.F;

	//currentAnimation = &run[(int)facingDirectionEnemy::SE];
	/*SetPivot(8, 32);
	size.create(13,36);*/

	SetPivot(32, 45);
	size.create(64, 64);
	this->position -= pivot;
}
