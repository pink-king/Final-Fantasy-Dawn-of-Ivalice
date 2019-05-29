#include "EnemyArcher.h" 
#include "j1EntityFactory.h"
#include "j1BuffManager.h"
#include "j1AttackManager.h"
EnemyArcher::EnemyArcher(const iPoint& pos, bool dummy) : Enemy(pos, 70, 10, 7, 5, 2.F, dummy, ENTITY_TYPE::ENEMY_ARCHER, "EnemyArcher")
{
	LoadAnims();
	App->audio->PlayFx(App->scene->golem_spawnSFX, 0);
}

EnemyArcher::~EnemyArcher()
{
	App->audio->PlayFx(App->scene->golem_deathSFX, 0);
}

bool EnemyArcher::PreUpdate()
{
	if (!isInDetectionRange())
		state = EnemyState::IDLE;

	if (to_die)
		state = EnemyState::DYING;
	return true;
}

bool EnemyArcher::Update(float dt)
{
	if (entityPushback)
	{
		DoPushback();
		entityPushback = false;
	}
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
		path_to_follow.clear();
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
			state = EnemyState::ATTACK;
			if (isOnMeleeRange())
			{
				currentAnimation = &meleeAttack[pointingDir];
				toAttackMelee = true;
			}
			else
			{
				currentAnimation = &basicAttack[pointingDir];
				toAttackMelee = false;
			}
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
		if ((int)currentAnimation->GetCurrentFloatFrame() == 2 && !attacked)
		{
			if (!toAttackMelee)
				App->entityFactory->CreateArrow(GetThrowingPos(), App->entityFactory->player->GetPivotPos(), 100, this, PROJECTILE_TYPE::GOLEM_ARROW);
			else
			{ 
				App->attackManager->AddPropagationAttack(this, GetSubtilePos(), propagationType::BFS, damageType::DIRECT, ELEMENTAL_TYPE::STONE_ELEMENT, baseDamage, 4, 60, true);
				// TODO: Add sfx
				App->camera2D->AddTrauma(0.2F);
				App->input->DoGamePadRumble(0.3F, 200);
			}
			attacked = true;
		}
		if (currentAnimation->Finished())
		{
			if (toAttackMelee)
				meleeAttack[pointingDir].Reset();
			else
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

bool EnemyArcher::isOnMeleeRange()
{
	return GetSubtilePos().DistanceTo(App->entityFactory->player->GetSubtilePos()) <= 2;
}

void EnemyArcher::LoadAnims()
{
	entityTex = App->entityFactory->enemyGolemTex;

	// TODO: import from xml

	idle[(int)facingDirectionEnemy::SE].PushBack({ 64, 928, 64, 64 }); //
	idle[(int)facingDirectionEnemy::S].PushBack({ 64, 864, 64, 64 }); //
	idle[(int)facingDirectionEnemy::SW].PushBack({ 64, 928, 64, 64 }); //
	idle[(int)facingDirectionEnemy::NE].PushBack({ 64, 1056, 64, 64 }); //
	idle[(int)facingDirectionEnemy::NW].PushBack({ 64, 1056, 64, 64 }); //
	idle[(int)facingDirectionEnemy::N].PushBack({ 64, 1120, 64, 64 }); //
	idle[(int)facingDirectionEnemy::W].PushBack({ 64, 992, 64, 64 });  //
	idle[(int)facingDirectionEnemy::E].PushBack({ 64, 992, 64, 64 });  //

	float animSpeed = 5.5F;

	run[(int)facingDirectionEnemy::SE].PushBack({ 0, 928, 64, 64 }); //
	run[(int)facingDirectionEnemy::SE].PushBack({ 64, 928, 64, 64 });
	run[(int)facingDirectionEnemy::SE].PushBack({ 128, 928, 64, 64 });
	run[(int)facingDirectionEnemy::SE].PushBack({ 64, 928, 64, 64 });
	run[(int)facingDirectionEnemy::SE].speed = animSpeed;

	run[(int)facingDirectionEnemy::SW].PushBack({ 0, 928, 64, 64 }); //
	run[(int)facingDirectionEnemy::SW].PushBack({ 64, 928, 64, 64 });
	run[(int)facingDirectionEnemy::SW].PushBack({ 128, 928, 64, 64 });
	run[(int)facingDirectionEnemy::SW].PushBack({ 64, 928, 64, 64 });
	run[(int)facingDirectionEnemy::SW].speed = animSpeed;

	run[(int)facingDirectionEnemy::S].PushBack({ 0, 864, 64, 64 }); //
	run[(int)facingDirectionEnemy::S].PushBack({ 64, 864, 64, 64 });
	run[(int)facingDirectionEnemy::S].PushBack({ 128, 864, 64, 64 });
	run[(int)facingDirectionEnemy::S].PushBack({ 64, 864, 64, 64 });
	run[(int)facingDirectionEnemy::S].speed = animSpeed;


	run[(int)facingDirectionEnemy::NE].PushBack({ 0, 1056, 64, 64 }); //
	run[(int)facingDirectionEnemy::NE].PushBack({ 64, 1056, 64, 64 });
	run[(int)facingDirectionEnemy::NE].PushBack({ 128, 1056, 64, 64 });
	run[(int)facingDirectionEnemy::NE].PushBack({ 64, 1056, 64, 64 });
	run[(int)facingDirectionEnemy::NE].speed = animSpeed;

	run[(int)facingDirectionEnemy::NW].PushBack({ 0, 1056, 64, 64 }); //
	run[(int)facingDirectionEnemy::NW].PushBack({ 64, 1056, 64, 64 });
	run[(int)facingDirectionEnemy::NW].PushBack({ 128, 1056, 64, 64 });
	run[(int)facingDirectionEnemy::NW].PushBack({ 64, 1056, 64, 64 });
	run[(int)facingDirectionEnemy::NW].speed = animSpeed;

	run[(int)facingDirectionEnemy::N].PushBack({ 0, 1120, 64, 64 }); //
	run[(int)facingDirectionEnemy::N].PushBack({ 64, 1120, 64, 64 });
	run[(int)facingDirectionEnemy::N].PushBack({ 128, 1120, 64, 64 });
	run[(int)facingDirectionEnemy::N].PushBack({ 64, 1120, 64, 64 });
	run[(int)facingDirectionEnemy::N].speed = animSpeed;

	run[(int)facingDirectionEnemy::E].PushBack({ 0, 992, 64, 64 }); //
	run[(int)facingDirectionEnemy::E].PushBack({ 64, 992, 64, 64 });
	run[(int)facingDirectionEnemy::E].PushBack({ 128, 992, 64, 64 });
	run[(int)facingDirectionEnemy::E].PushBack({ 64, 992, 64, 64 });
	run[(int)facingDirectionEnemy::E].speed = animSpeed;

	run[(int)facingDirectionEnemy::W].PushBack({ 0, 992, 64, 64 }); //
	run[(int)facingDirectionEnemy::W].PushBack({ 64, 992, 64, 64 });
	run[(int)facingDirectionEnemy::W].PushBack({ 128, 992, 64, 64 });
	run[(int)facingDirectionEnemy::W].PushBack({ 64, 992, 64, 64 });
	run[(int)facingDirectionEnemy::W].speed = animSpeed;


	// --------------------- Attacks -----------------------------
	float attackSpeedAnim = attackSpeed * 4.F;

	basicAttack[(int)facingDirectionEnemy::SE].PushBack({ 0, 576, 64, 64 }); //
	basicAttack[(int)facingDirectionEnemy::SE].PushBack({ 64, 576, 64, 64 });
	basicAttack[(int)facingDirectionEnemy::SE].PushBack({ 128, 576, 64, 64 });
	basicAttack[(int)facingDirectionEnemy::SE].PushBack({ 192, 576, 64, 64 });
	basicAttack[(int)facingDirectionEnemy::SE].loop = false;
	basicAttack[(int)facingDirectionEnemy::SE].speed = attackSpeedAnim;

	basicAttack[(int)facingDirectionEnemy::SW].PushBack({ 0, 576, 64, 64 }); //
	basicAttack[(int)facingDirectionEnemy::SW].PushBack({ 64, 576, 64, 64 });
	basicAttack[(int)facingDirectionEnemy::SW].PushBack({ 128, 576, 64, 64 });
	basicAttack[(int)facingDirectionEnemy::SW].PushBack({ 192, 576, 64, 64 });
	basicAttack[(int)facingDirectionEnemy::SW].loop = false;
	basicAttack[(int)facingDirectionEnemy::SW].speed = attackSpeedAnim;

	basicAttack[(int)facingDirectionEnemy::S].PushBack({ 0, 512, 64, 64 }); //
	basicAttack[(int)facingDirectionEnemy::S].PushBack({ 64, 512, 64, 64 });
	basicAttack[(int)facingDirectionEnemy::S].PushBack({ 128, 512, 64, 64 });
	basicAttack[(int)facingDirectionEnemy::S].PushBack({ 192, 512, 64, 64 });
	basicAttack[(int)facingDirectionEnemy::S].loop = false;
	basicAttack[(int)facingDirectionEnemy::S].speed = attackSpeedAnim;

	basicAttack[(int)facingDirectionEnemy::N].PushBack({ 0, 768, 64, 64 }); //
	basicAttack[(int)facingDirectionEnemy::N].PushBack({ 64, 768, 64, 64 });
	basicAttack[(int)facingDirectionEnemy::N].PushBack({ 128, 768, 64, 64 });
	basicAttack[(int)facingDirectionEnemy::N].PushBack({ 192, 768, 64, 64 });
	basicAttack[(int)facingDirectionEnemy::N].loop = false;
	basicAttack[(int)facingDirectionEnemy::N].speed = attackSpeedAnim;

	basicAttack[(int)facingDirectionEnemy::NE].PushBack({ 0, 704, 64, 64 }); //
	basicAttack[(int)facingDirectionEnemy::NE].PushBack({ 64, 704, 64, 64 });
	basicAttack[(int)facingDirectionEnemy::NE].PushBack({ 128, 704, 64, 64 });
	basicAttack[(int)facingDirectionEnemy::NE].PushBack({ 192, 704, 64, 64 });
	basicAttack[(int)facingDirectionEnemy::NE].loop = false;
	basicAttack[(int)facingDirectionEnemy::NE].speed = attackSpeedAnim;


	basicAttack[(int)facingDirectionEnemy::NW].PushBack({ 0, 704, 64, 64 }); //
	basicAttack[(int)facingDirectionEnemy::NW].PushBack({ 64, 704, 64, 64 });
	basicAttack[(int)facingDirectionEnemy::NW].PushBack({ 128, 704, 64, 64 });
	basicAttack[(int)facingDirectionEnemy::NW].PushBack({ 192, 704, 64, 64 });
	basicAttack[(int)facingDirectionEnemy::NW].loop = false;
	basicAttack[(int)facingDirectionEnemy::NW].speed = attackSpeedAnim;

	basicAttack[(int)facingDirectionEnemy::E].PushBack({ 0, 640, 64, 64 }); //
	basicAttack[(int)facingDirectionEnemy::E].PushBack({ 64, 640, 64, 64 });
	basicAttack[(int)facingDirectionEnemy::E].PushBack({ 128, 640, 64, 64 });
	basicAttack[(int)facingDirectionEnemy::E].PushBack({ 192, 640, 64, 64 });
	basicAttack[(int)facingDirectionEnemy::E].loop = false;
	basicAttack[(int)facingDirectionEnemy::E].speed = attackSpeedAnim;

	basicAttack[(int)facingDirectionEnemy::W].PushBack({ 0, 640, 64, 64 }); //
	basicAttack[(int)facingDirectionEnemy::W].PushBack({ 64, 640, 64, 64 });
	basicAttack[(int)facingDirectionEnemy::W].PushBack({ 128, 640, 64, 64 });
	basicAttack[(int)facingDirectionEnemy::W].PushBack({ 192, 640, 64, 64 });
	basicAttack[(int)facingDirectionEnemy::W].loop = false;
	basicAttack[(int)facingDirectionEnemy::W].speed = attackSpeedAnim;

	// --------- Melee
	attackSpeedAnim = attackSpeed * 3.F;

	meleeAttack[(int)facingDirectionEnemy::SE].PushBack({ 0, 64, 64, 64 }); //
	meleeAttack[(int)facingDirectionEnemy::SE].PushBack({ 64, 64, 64, 64 });
	meleeAttack[(int)facingDirectionEnemy::SE].PushBack({ 128, 64, 64, 64 });
	meleeAttack[(int)facingDirectionEnemy::SE].PushBack({ 192, 64, 64, 64 });
	meleeAttack[(int)facingDirectionEnemy::SE].loop = false;
	meleeAttack[(int)facingDirectionEnemy::SE].speed = attackSpeedAnim;

	meleeAttack[(int)facingDirectionEnemy::SW].PushBack({ 0, 64, 64, 64 }); //
	meleeAttack[(int)facingDirectionEnemy::SW].PushBack({ 64, 64, 64, 64 });
	meleeAttack[(int)facingDirectionEnemy::SW].PushBack({ 128, 64, 64, 64 });
	meleeAttack[(int)facingDirectionEnemy::SW].PushBack({ 192, 64, 64, 64 });
	meleeAttack[(int)facingDirectionEnemy::SW].loop = false;
	meleeAttack[(int)facingDirectionEnemy::SW].speed = attackSpeedAnim;

	meleeAttack[(int)facingDirectionEnemy::S].PushBack({ 0, 0, 64, 64 }); //
	meleeAttack[(int)facingDirectionEnemy::S].PushBack({ 64, 0, 64, 64 });
	meleeAttack[(int)facingDirectionEnemy::S].PushBack({ 128, 0, 64, 64 });
	meleeAttack[(int)facingDirectionEnemy::S].PushBack({ 192, 0, 64, 64 });
	meleeAttack[(int)facingDirectionEnemy::S].loop = false;
	meleeAttack[(int)facingDirectionEnemy::S].speed = attackSpeedAnim;

	meleeAttack[(int)facingDirectionEnemy::N].PushBack({ 0, 256, 64, 64 }); //
	meleeAttack[(int)facingDirectionEnemy::N].PushBack({ 64, 256, 64, 64 });
	meleeAttack[(int)facingDirectionEnemy::N].PushBack({ 128, 256, 64, 64 });
	meleeAttack[(int)facingDirectionEnemy::N].PushBack({ 192, 256, 64, 64 });
	meleeAttack[(int)facingDirectionEnemy::N].loop = false;
	meleeAttack[(int)facingDirectionEnemy::N].speed = attackSpeedAnim;

	meleeAttack[(int)facingDirectionEnemy::NE].PushBack({ 0, 192, 64, 64 }); //
	meleeAttack[(int)facingDirectionEnemy::NE].PushBack({ 64, 192, 64, 64 });
	meleeAttack[(int)facingDirectionEnemy::NE].PushBack({ 128, 192, 64, 64 });
	meleeAttack[(int)facingDirectionEnemy::NE].PushBack({ 192, 192, 64, 64 });
	meleeAttack[(int)facingDirectionEnemy::NE].loop = false;
	meleeAttack[(int)facingDirectionEnemy::NE].speed = attackSpeedAnim;

	meleeAttack[(int)facingDirectionEnemy::NW].PushBack({ 0, 192, 64, 64 }); //
	meleeAttack[(int)facingDirectionEnemy::NW].PushBack({ 64, 192, 64, 64 });
	meleeAttack[(int)facingDirectionEnemy::NW].PushBack({ 128, 192, 64, 64 });
	meleeAttack[(int)facingDirectionEnemy::NW].PushBack({ 192, 192, 64, 64 });
	meleeAttack[(int)facingDirectionEnemy::NW].loop = false;
	meleeAttack[(int)facingDirectionEnemy::NW].speed = attackSpeedAnim;

	meleeAttack[(int)facingDirectionEnemy::E].PushBack({ 0, 128, 64, 64 }); //
	meleeAttack[(int)facingDirectionEnemy::E].PushBack({ 64, 128, 64, 64 });
	meleeAttack[(int)facingDirectionEnemy::E].PushBack({ 128, 128, 64, 64 });
	meleeAttack[(int)facingDirectionEnemy::E].PushBack({ 192, 128, 64, 64 });
	meleeAttack[(int)facingDirectionEnemy::E].loop = false;
	meleeAttack[(int)facingDirectionEnemy::E].speed = attackSpeedAnim;

	meleeAttack[(int)facingDirectionEnemy::W].PushBack({ 0, 128, 64, 64 }); //
	meleeAttack[(int)facingDirectionEnemy::W].PushBack({ 64, 128, 64, 64 });
	meleeAttack[(int)facingDirectionEnemy::W].PushBack({ 128, 128, 64, 64 });
	meleeAttack[(int)facingDirectionEnemy::W].PushBack({ 192, 128, 64, 64 });
	meleeAttack[(int)facingDirectionEnemy::W].loop = false;
	meleeAttack[(int)facingDirectionEnemy::W].speed = attackSpeedAnim;

	// -------------

	// Hardcode mode 
	dyingAnim.PushBack({ 0, 320, 64, 64 });
	dyingAnim.PushBack({ 0, 320, 64, 64 });
	dyingAnim.PushBack({ 0, 320, 64, 64 });
	dyingAnim.PushBack({ 0, 320, 64, 64 });
	dyingAnim.PushBack({ 0, 448, 64, 64 });
	dyingAnim.PushBack({ 0, 448, 64, 64 });
	dyingAnim.PushBack({ 0, 448, 64, 64 });
	dyingAnim.PushBack({ 0, 448, 64, 64 });
	dyingAnim.loop = false;
	dyingAnim.speed = 5.F;

	//currentAnimation = &run[(int)facingDirectionEnemy::SE];
	/*SetPivot(8, 32);
	size.create(13,36);*/

	SetPivot(32, 45);
	size.create(64, 64);
	this->position -= pivot;
}