#include "EnemyTest.h"
#include "j1Textures.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1Map.h"
#include "j1EntityFactory.h"
#include "PlayerEntity.h"
#include "j1PathFinding.h"
//#include "j1AttackManager.h"
#include "UiItem_HitPointManager.h"
#include "UiItem_Image.h"
#include "j1BuffManager.h"


#include <random>

EnemyTest::EnemyTest(iPoint position, uint speed, uint detectionRange, uint attackRange, uint baseDamage, float attackSpeed) : Enemy(position, speed, detectionRange, attackRange, baseDamage, attackSpeed, false, ENTITY_TYPE::ENEMY_TEST, "Enemy Test")
{
	to_die = false;
	LoadAnims(); 
	this->isDynamicEnemy = true;
}

// Standard stats

EnemyTest::EnemyTest(iPoint position, bool dummy) : Enemy(position, 100, 10, 1, 10, 1.3F, dummy, ENTITY_TYPE::ENEMY_TEST, "Enemy Test")
{
	to_die = false;
	LoadAnims();
	this->isDynamicEnemy = true;
}

EnemyTest::~EnemyTest()
{
	App->audio->PlayFx(App->scene->goblinDeath, 0);
	LOG("Bye Enemy Test ");
} 

bool EnemyTest::Start()
{


	
	return true;
}

bool EnemyTest::PreUpdate()
{
	if (!isInDetectionRange())
		state = EnemyState::IDLE;

	if (to_die)
		state = EnemyState::DYING;
	return true;
}

bool EnemyTest::Update(float dt)
{
	/*hitPoint->attachedEntity->name;

	if (hitPoint != nullptr)
	{
		LOG("");
	}*/
	if (entityPushback)
	{
		DoPushback();
		entityPushback = false;
	}
	if (!isParalize)
	{
	
		SetState(dt);

		/*if (GetRandomValue(1, 10000) == 900)
			App->audio->PlayFx(App->entityFactory->goblinLaugh, 0);*/
		// --------------------------------------------------------------------------- This is faked: recieve attack from player

		/*if (App->input->GetControllerButton(SDL_CONTROLLER_BUTTON_X) == KEY_DOWN)
		{
			life -= 20;
			App->HPManager->callHPLabelSpawn(this, 20, ELEMENTAL_TYPE::FIRE_ELEMENT);

		}*/

		if (App->input->GetKey(SDL_SCANCODE_7) == KEY_DOWN)
		{
			life -= 20;
			//App->HPManager->callHPLabelSpawn(iPoint(this->position.x, this->position.y), 20, ELEMENTAL_TYPE::FIRE_ELEMENT);

		}

		/*if (App->input->GetControllerButton(SDL_CONTROLLER_BUTTON_Y) == KEY_DOWN)
		{
			App->buff->DirectAttack(App->entityFactory->player->selectedCharacterEntity, this, 30, ELEMENTAL_TYPE::ICE_ELEMENT, "meh");
			App->HPManager->callHPLabelSpawn(iPoint(this->position.x, this->position.y), 30, ELEMENTAL_TYPE::POISON_ELEMENT);
		}*/
	}

	if (stat.size() != 0 && !App->pause)
	{
		if (App->buff->DamageInTime(this))
		{
			App->buff->entitiesTimeDamage.remove(this);
		}
	}
	/*if (life <= 0)
	{
		CleanUp(); 
	}*/

	// --------------------------------------------------------------------------- This is faked: recieve attack from player

	return true;
}

bool EnemyTest::PostUpdate()
{
	

	return true;
}


void EnemyTest::SetState(float dt)
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

		if (path_to_follow.size() <= 0)
		{
			if (isInAttackRange())
			{
				SetLookingTo(App->entityFactory->player->GetPivotPos());
				//currentAnimation = &idle[pointingDir];
				state = EnemyState::ATTACK;
				currentAnimation = &basicAttack[pointingDir];
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
		
		if (/*checkTime.ReadSec() > attackPerS &&*/ (int)currentAnimation->GetCurrentFloatFrame() == 2 && !attacked )
		{
			App->attackManager->AddPropagationAttack(this, GetSubtilePos(), propagationType::BFS,
				damageType::DIRECT, ELEMENTAL_TYPE::POISON_ELEMENT, baseDamage, 4, 50, false);			
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
			//LOG("Gave a free pass!");
		}
		break;
	}

	case EnemyState::MENTAL_EMBOLIA:
	{
		currentAnimation = &idle[pointingDir];
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

bool EnemyTest::CleanUp()
{
	if (path_to_follow.empty() == false)
		FreeMyReservedAdjacents();

	path_to_follow.clear();

	std::list<entityStat*>::iterator item = stat.begin();
	for (; item != stat.end(); ++item)
	{
		stat.remove(*item);
		delete *item;
		*item = nullptr;
	}
	stat.clear();


	// Textures Loaded and Unloaded in Entity Factory

	/*if (entityTex != nullptr)
	{
		App->tex->UnLoad(entityTex);
		entityTex = nullptr;
	}*/

	/*if (debugSubtile != nullptr)
	{
		App->tex->UnLoad(debugSubtile);
		debugSubtile = nullptr;
	}*/

	return true;
}

void EnemyTest::LoadAnims()
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

	dyingAnim.PushBack({ 12, 129, 18, 29});
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
