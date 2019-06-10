#include "EnemyBomb.h"
#include "j1Textures.h"
#include "j1EntityFactory.h"
#include "j1Map.h"
#include "j1ParticlesClassic.h"
#include "j1BuffManager.h"

EnemyBomb::EnemyBomb(iPoint position, uint speed, uint detectionRange, uint attackRange, uint baseDamage) : Enemy(position, speed, detectionRange, attackRange, baseDamage, 0, false, ENTITY_TYPE::ENEMY_BOMB, "Enemy Bomb")
{
	// TODO: Load From XML
	LoadAnims();

	this->isDynamicEnemy = true;
}

// Standard stats
EnemyBomb::EnemyBomb(iPoint position, bool dummy) : Enemy(position, 120, 12, 1, 10, 1.F, dummy, ENTITY_TYPE::ENEMY_BOMB, "Bomb")
{
	LoadAnims(); 

	this->isDynamicEnemy = true;
}

EnemyBomb::~EnemyBomb()
{
//	App->audio->PlayFx(App->entityFactory->BombDeathSFX, 0);
	App->attackManager->AddPropagationAttack(this, GetSubtilePos(), propagationType::BFS, damageType::DIRECT, ELEMENTAL_TYPE::FIRE_ELEMENT, 3, 2, 60, true);



}

bool EnemyBomb::Start()
{
	App->audio->PlayFx(App->scene->BombDeathSFX, 0);

	
	return true;
}
bool EnemyBomb::PreUpdate()
{

	if (!isInDetectionRange())
		state = EnemyState::IDLE;

	if (to_die)
		state = EnemyState::DYING;


	return true;
}
bool EnemyBomb::Update(float dt)
{
	if (entityPushback)
	{
		DoPushback();
		entityPushback = false;
	}

	if(!isParalize)
		SetState(dt);
	
	if (stat.size() != 0 && !App->pause)
	{
		if (App->buff->DamageInTime(this))
		{
			App->buff->entitiesTimeDamage.remove(this);
		}
	}
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
	for (; item != stat.end(); )
	{
		delete *item;
		*item = nullptr;
		item = stat.erase(item);

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
		path_to_follow.clear();
		currentAnimation = &idle[pointingDir];
		if (isInDetectionRange() && !dummy)
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
			App->particles->AddParticle(App->particles->explosion01, position.x - 10, position.y - 10);
			App->particles->AddParticle(App->particles->explosion03, position.x - 12, position.y - 10); // Nice combo here
			App->audio->PlayFx(App->scene->bombExplodeSFX, 0);
			App->attackManager->AddPropagationAttack(this, GetSubtilePos(), propagationType::BFS,
				damageType::DIRECT, ELEMENTAL_TYPE::FIRE_ELEMENT, baseDamage, level + 6, 20, true);		
			exploded = true; 
			to_die = true;
		}
	}
	break;

	case EnemyState::DYING:
		currentAnimation = &dyingAnim;
		if (!exploded)
		{
			App->particles->AddParticle(App->particles->explosion01, position.x - 10, position.y - 10);
			App->particles->AddParticle(App->particles->explosion03, position.x - 12, position.y - 10); // Nice combo here

			App->attackManager->AddPropagationAttack(this, GetSubtilePos(), propagationType::BFS,
				damageType::DIRECT, ELEMENTAL_TYPE::FIRE_ELEMENT, baseDamage, level + 6, 40, true);
			App->audio->PlayFx(App->scene->bombExplodeSFX, 0);
			exploded = true;
		}

		if (currentAnimation->Finished()) {
			to_delete = true;
		}
		break;


	case EnemyState::MENTAL_EMBOLIA:
	{
		currentAnimation = &idle[pointingDir];
		break;
	}

	default:
		break;
	}
}

void EnemyBomb::LoadAnims()
{
	entityTex = App->entityFactory->enemyBombTex;

	idle[(int)facingDirectionEnemy::SE].PushBack({ 0, 0, 32, 32 });
	idle[(int)facingDirectionEnemy::S].PushBack({ 0, 32, 32, 32 });
	idle[(int)facingDirectionEnemy::SW].PushBack({ 0, 0, 32, 32 });
	idle[(int)facingDirectionEnemy::NE].PushBack({ 0, 64, 32, 32 });
	idle[(int)facingDirectionEnemy::NW].PushBack({ 0, 64, 32, 32 });
	idle[(int)facingDirectionEnemy::N].PushBack({ 0, 96, 32, 32 });
	idle[(int)facingDirectionEnemy::W].PushBack({ 0, 128, 32, 32 });
	idle[(int)facingDirectionEnemy::E].PushBack({ 0, 128, 32, 32 });

	// Run anim speed
	float animSpeed = 10.F;

	run[(int)facingDirectionEnemy::SE].PushBack({ 0, 0, 32, 32 });
	run[(int)facingDirectionEnemy::SE].PushBack({ 32, 0, 32, 32 });
	run[(int)facingDirectionEnemy::SE].PushBack({ 64, 0, 32, 32 });
	run[(int)facingDirectionEnemy::SE].PushBack({ 32, 0, 32, 32 });
	run[(int)facingDirectionEnemy::SE].speed = animSpeed;

	run[(int)facingDirectionEnemy::SW].PushBack({ 0, 0, 32, 32 });
	run[(int)facingDirectionEnemy::SW].PushBack({ 32, 0, 32, 32 });
	run[(int)facingDirectionEnemy::SW].PushBack({ 64, 0, 32, 32 });
	run[(int)facingDirectionEnemy::SW].PushBack({ 32, 0, 32, 32 });
	run[(int)facingDirectionEnemy::SW].speed = animSpeed;

	run[(int)facingDirectionEnemy::S].PushBack({ 0, 32, 32, 32 });
	run[(int)facingDirectionEnemy::S].PushBack({ 32, 32, 32, 32 });
	run[(int)facingDirectionEnemy::S].PushBack({ 64, 32, 32, 32 });
	run[(int)facingDirectionEnemy::S].PushBack({ 32, 32, 32, 32 });
	run[(int)facingDirectionEnemy::S].speed = animSpeed;


	run[(int)facingDirectionEnemy::NE].PushBack({ 0, 64, 32, 32 });
	run[(int)facingDirectionEnemy::NE].PushBack({ 32, 64, 32, 32 });
	run[(int)facingDirectionEnemy::NE].PushBack({ 64, 64, 32, 32 });
	run[(int)facingDirectionEnemy::NE].PushBack({ 32, 64, 32, 32 });
	run[(int)facingDirectionEnemy::NE].speed = animSpeed;

	run[(int)facingDirectionEnemy::NW].PushBack({ 0, 64, 32, 32 });
	run[(int)facingDirectionEnemy::NW].PushBack({ 32, 64, 32, 32 });
	run[(int)facingDirectionEnemy::NW].PushBack({ 64, 64, 32, 32 });
	run[(int)facingDirectionEnemy::NW].PushBack({ 32, 64, 32, 32 });
	run[(int)facingDirectionEnemy::NW].speed = animSpeed;

	run[(int)facingDirectionEnemy::N].PushBack({ 0, 96, 32, 32 });
	run[(int)facingDirectionEnemy::N].PushBack({ 32, 96, 32, 32 });
	run[(int)facingDirectionEnemy::N].PushBack({ 64, 96, 32, 32 });
	run[(int)facingDirectionEnemy::N].PushBack({ 32, 96, 32, 32 });
	run[(int)facingDirectionEnemy::N].speed = animSpeed;

	run[(int)facingDirectionEnemy::E].PushBack({ 0, 128, 32, 32 });
	run[(int)facingDirectionEnemy::E].PushBack({ 32, 128, 32, 32 });
	run[(int)facingDirectionEnemy::E].PushBack({ 64, 128, 32, 32 });
	run[(int)facingDirectionEnemy::E].PushBack({ 32, 128, 32, 32 });
	run[(int)facingDirectionEnemy::E].speed = animSpeed;

	run[(int)facingDirectionEnemy::W].PushBack({ 0, 128, 32, 32 });
	run[(int)facingDirectionEnemy::W].PushBack({ 32, 128, 32, 32 });
	run[(int)facingDirectionEnemy::W].PushBack({ 64, 128, 32, 32 });
	run[(int)facingDirectionEnemy::W].PushBack({ 32, 128, 32, 32 });
	run[(int)facingDirectionEnemy::W].speed = animSpeed;

	// Basic attack anim Speed
	animSpeed = 8.F;

	basicAttack[(int)facingDirectionEnemy::SW].PushBack({ 96, 0, 32, 32 });
	basicAttack[(int)facingDirectionEnemy::SW].PushBack({ 128, 0, 32, 32 });
	basicAttack[(int)facingDirectionEnemy::SW].PushBack({ 160, 0, 32, 32 });
	basicAttack[(int)facingDirectionEnemy::SW].PushBack({ 192, 0, 32, 32 });
	basicAttack[(int)facingDirectionEnemy::SW].loop = false;
	basicAttack[(int)facingDirectionEnemy::SW].speed = animSpeed;

	basicAttack[(int)facingDirectionEnemy::SE].PushBack({ 96, 0, 32, 32 });
	basicAttack[(int)facingDirectionEnemy::SE].PushBack({ 128, 0, 32, 32 });
	basicAttack[(int)facingDirectionEnemy::SE].PushBack({ 160, 0, 32, 32 });
	basicAttack[(int)facingDirectionEnemy::SE].PushBack({ 192, 0, 32, 32 });
	basicAttack[(int)facingDirectionEnemy::SE].loop = false;
	basicAttack[(int)facingDirectionEnemy::SE].speed = animSpeed;

	basicAttack[(int)facingDirectionEnemy::S].PushBack({ 96, 32, 32, 32 });
	basicAttack[(int)facingDirectionEnemy::S].PushBack({ 128, 32, 32, 32 });
	basicAttack[(int)facingDirectionEnemy::S].PushBack({ 160, 32, 32, 32 });
	basicAttack[(int)facingDirectionEnemy::S].PushBack({ 192, 32, 32, 32 });
	basicAttack[(int)facingDirectionEnemy::S].loop = false;
	basicAttack[(int)facingDirectionEnemy::S].speed = animSpeed;

	basicAttack[(int)facingDirectionEnemy::N].PushBack({ 96, 96, 32, 32 });
	basicAttack[(int)facingDirectionEnemy::N].PushBack({ 128, 96, 32, 32 });
	basicAttack[(int)facingDirectionEnemy::N].PushBack({ 160, 96, 32, 32 });
	basicAttack[(int)facingDirectionEnemy::N].PushBack({ 192, 96, 32, 32 });
	basicAttack[(int)facingDirectionEnemy::N].loop = false;
	basicAttack[(int)facingDirectionEnemy::N].speed = animSpeed;

	basicAttack[(int)facingDirectionEnemy::NE].PushBack({ 96, 64, 32, 32 });
	basicAttack[(int)facingDirectionEnemy::NE].PushBack({ 128, 64, 32, 32 });
	basicAttack[(int)facingDirectionEnemy::NE].PushBack({ 160, 64, 32, 32 });
	basicAttack[(int)facingDirectionEnemy::NE].PushBack({ 192, 64, 32, 32 });
	basicAttack[(int)facingDirectionEnemy::NE].loop = false;
	basicAttack[(int)facingDirectionEnemy::NE].speed = animSpeed;

	basicAttack[(int)facingDirectionEnemy::NW].PushBack({ 96, 64, 32, 32 });
	basicAttack[(int)facingDirectionEnemy::NW].PushBack({ 128, 64, 32, 32 });
	basicAttack[(int)facingDirectionEnemy::NW].PushBack({ 160, 64, 32, 32 });
	basicAttack[(int)facingDirectionEnemy::NW].PushBack({ 192, 64, 32, 32 });
	basicAttack[(int)facingDirectionEnemy::NW].loop = false;
	basicAttack[(int)facingDirectionEnemy::NW].speed = animSpeed;


	basicAttack[(int)facingDirectionEnemy::E].PushBack({ 96, 128, 32, 32 });
	basicAttack[(int)facingDirectionEnemy::E].PushBack({ 128, 128, 32, 32 });
	basicAttack[(int)facingDirectionEnemy::E].PushBack({ 160, 128, 32, 32 });
	basicAttack[(int)facingDirectionEnemy::E].PushBack({ 192, 128, 32, 32 });
	basicAttack[(int)facingDirectionEnemy::E].loop = false;
	basicAttack[(int)facingDirectionEnemy::E].speed = animSpeed;

	basicAttack[(int)facingDirectionEnemy::W].PushBack({ 96, 128, 32, 32 });
	basicAttack[(int)facingDirectionEnemy::W].PushBack({ 128, 128, 32, 32 });
	basicAttack[(int)facingDirectionEnemy::W].PushBack({ 160, 128, 32, 32 });
	basicAttack[(int)facingDirectionEnemy::W].PushBack({ 192, 128, 32, 32 });
	basicAttack[(int)facingDirectionEnemy::W].loop = false;
	basicAttack[(int)facingDirectionEnemy::W].speed = animSpeed;


	// I think these two are "damaged" but fit well
	/*dyingAnim.PushBack({ 150, 3, 15, 22});
	dyingAnim.PushBack({ 166, 2, 15, 22 });*/

	dyingAnim.PushBack({ 0, 160, 32, 32 });
	dyingAnim.PushBack({ 32, 160, 32, 32 });
	dyingAnim.PushBack({ 64, 160, 32, 32 });
	dyingAnim.PushBack({ 96, 160, 32, 32 });
	dyingAnim.loop = false;
	dyingAnim.speed = 10.F;

	SetPivot(15, 25);
	size.create(32, 32);
	this->position -= pivot;
}
