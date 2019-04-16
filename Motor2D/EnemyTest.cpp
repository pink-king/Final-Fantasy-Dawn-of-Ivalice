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

#include <random>

EnemyTest::EnemyTest(iPoint position, uint speed, uint detectionRange, uint attackRange, uint baseDamage, float attackSpeed) : Enemy(position, speed, detectionRange, attackRange, baseDamage, attackSpeed, ENTITY_TYPE::ENEMY_TEST, "Enemy Test")
{
	name.assign("Test");

	// TODO: import from xml
	entityTex = App->tex->Load("textures/enemies/enemygoblin.png"); // TODO: note, repetead entities/enemies, load texture once time
	debugSubtile = App->tex->Load("maps/tile_32x32_2.png");

	idle[(int)facingDirectionEnemy::SE].PushBack({ 29, 14, 16, 28 });
	idle[(int)facingDirectionEnemy::S].PushBack({ 29, 47, 16, 27 });
	idle[(int)facingDirectionEnemy::SW].PushBack({ 29, 14, 16, 28 });
	idle[(int)facingDirectionEnemy::NE].PushBack({ 29, 80, 16, 28 });
	idle[(int)facingDirectionEnemy::NW].PushBack({ 29, 80, 16, 28 });
	idle[(int)facingDirectionEnemy::N].PushBack({ 29, 113, 16, 28 });
	idle[(int)facingDirectionEnemy::W].PushBack({ 13, 145, 17, 22 });
	idle[(int)facingDirectionEnemy::E].PushBack({ 13, 145, 17, 22 });



	run[(int)facingDirectionEnemy::SE].PushBack({ 12, 14, 16, 28 });
	run[(int)facingDirectionEnemy::SE].PushBack({ 29, 14, 16, 28 });
	run[(int)facingDirectionEnemy::SE].PushBack({ 46, 14, 16, 28 });
	run[(int)facingDirectionEnemy::SE].PushBack({ 29, 14, 16, 28 });
	run[(int)facingDirectionEnemy::SE].speed = 7.F;

	run[(int)facingDirectionEnemy::S].PushBack({ 12, 48, 16, 27 });
	run[(int)facingDirectionEnemy::S].PushBack({ 29, 47, 16, 27 });
	run[(int)facingDirectionEnemy::S].PushBack({ 46, 48, 16, 27 });
	run[(int)facingDirectionEnemy::S].PushBack({ 29, 47, 16, 27 });
	run[(int)facingDirectionEnemy::S].speed = 7.F;

	// Same as SE - flipped
	run[(int)facingDirectionEnemy::SW].PushBack({ 12, 14, 16, 28 });
	run[(int)facingDirectionEnemy::SW].PushBack({ 29, 14, 16, 28 });
	run[(int)facingDirectionEnemy::SW].PushBack({ 46, 14, 16, 28 });
	run[(int)facingDirectionEnemy::SW].PushBack({ 29, 14, 16, 28 });
	run[(int)facingDirectionEnemy::SW].speed = 7.F;
		
	run[(int)facingDirectionEnemy::NE].PushBack({ 12, 81, 16, 27 });
	run[(int)facingDirectionEnemy::NE].PushBack({ 29, 80, 16, 28 });
	run[(int)facingDirectionEnemy::NE].PushBack({ 46, 81, 16, 26 });
	run[(int)facingDirectionEnemy::NE].PushBack({ 29, 80, 16, 28 });
	run[(int)facingDirectionEnemy::NE].speed = 7.F;

	run[(int)facingDirectionEnemy::NW].PushBack({ 12, 81, 16, 27 });
	run[(int)facingDirectionEnemy::NW].PushBack({ 29, 80, 16, 28 });
	run[(int)facingDirectionEnemy::NW].PushBack({ 46, 81, 16, 26 });
	run[(int)facingDirectionEnemy::NW].PushBack({ 29, 80, 16, 28 });
	run[(int)facingDirectionEnemy::NW].speed = 7.F;

	run[(int)facingDirectionEnemy::N].PushBack({ 12, 114, 16, 28 });
	run[(int)facingDirectionEnemy::N].PushBack({ 29, 113, 16, 28 });
	run[(int)facingDirectionEnemy::N].PushBack({ 46, 114, 16, 28 });
	run[(int)facingDirectionEnemy::N].PushBack({ 29, 113, 16, 28 });
	run[(int)facingDirectionEnemy::N].speed = 7.F;

	run[(int)facingDirectionEnemy::E].PushBack({ 13, 145, 17, 22 });

	run[(int)facingDirectionEnemy::W].PushBack({ 13, 145, 17, 22 });

	float attackSpeedAnim = attackSpeed * 4; 

	basicAttack[(int)facingDirectionEnemy::SE].PushBack({ 127, 18, 15, 27 });
	basicAttack[(int)facingDirectionEnemy::SE].PushBack({ 144, 18, 16, 27 });
	basicAttack[(int)facingDirectionEnemy::SE].PushBack({ 161, 15, 16, 29 });
	basicAttack[(int)facingDirectionEnemy::SE].PushBack({ 179, 17, 15, 27 });
	basicAttack[(int)facingDirectionEnemy::SE].loop = false; 
	basicAttack[(int)facingDirectionEnemy::SE].speed = attackSpeedAnim;

	basicAttack[(int)facingDirectionEnemy::SW].PushBack({ 127, 18, 15, 27 });
	basicAttack[(int)facingDirectionEnemy::SW].PushBack({ 144, 18, 16, 27 });
	basicAttack[(int)facingDirectionEnemy::SW].PushBack({ 161, 15, 16, 29 });
	basicAttack[(int)facingDirectionEnemy::SW].PushBack({ 179, 17, 15, 27 });
	basicAttack[(int)facingDirectionEnemy::SW].loop = false;
	basicAttack[(int)facingDirectionEnemy::SW].speed = attackSpeedAnim;

	basicAttack[(int)facingDirectionEnemy::S].PushBack({ 128, 48, 15, 29 });
	basicAttack[(int)facingDirectionEnemy::S].PushBack({ 144, 47, 16, 29 });
	basicAttack[(int)facingDirectionEnemy::S].PushBack({ 161, 51, 15, 27 });
	basicAttack[(int)facingDirectionEnemy::S].PushBack({ 178, 50, 15, 28 });
	basicAttack[(int)facingDirectionEnemy::S].loop = false;
	basicAttack[(int)facingDirectionEnemy::S].speed = attackSpeedAnim;

	basicAttack[(int)facingDirectionEnemy::N].PushBack({ 128, 114, 15, 29 });
	basicAttack[(int)facingDirectionEnemy::N].PushBack({ 145, 113, 15, 29 });
	basicAttack[(int)facingDirectionEnemy::N].PushBack({ 161, 116, 16, 27 });
	basicAttack[(int)facingDirectionEnemy::N].PushBack({ 178, 115, 16, 28 });
	basicAttack[(int)facingDirectionEnemy::N].loop = false;
	basicAttack[(int)facingDirectionEnemy::N].speed = attackSpeedAnim;

	basicAttack[(int)facingDirectionEnemy::NE].PushBack({ 127, 81, 16, 28 });
	basicAttack[(int)facingDirectionEnemy::NE].PushBack({ 144, 81, 16, 28 });
	basicAttack[(int)facingDirectionEnemy::NE].PushBack({ 161, 83, 16, 27 });
	basicAttack[(int)facingDirectionEnemy::NE].PushBack({ 178, 82, 16, 28 });
	basicAttack[(int)facingDirectionEnemy::NE].speed = attackSpeedAnim; 
	basicAttack[(int)facingDirectionEnemy::NE].loop = false;


	basicAttack[(int)facingDirectionEnemy::NW].PushBack({ 127, 81, 16, 28 });
	basicAttack[(int)facingDirectionEnemy::NW].PushBack({ 144, 81, 16, 28 });
	basicAttack[(int)facingDirectionEnemy::NW].PushBack({ 161, 83, 16, 27 });
	basicAttack[(int)facingDirectionEnemy::NW].PushBack({ 178, 82, 16, 28 });
	basicAttack[(int)facingDirectionEnemy::NW].speed = attackSpeedAnim;
	basicAttack[(int)facingDirectionEnemy::NW].loop = false;

	basicAttack[(int)facingDirectionEnemy::E].PushBack({ 128, 147, 13, 19 });
	basicAttack[(int)facingDirectionEnemy::E].speed = attackSpeedAnim;
	basicAttack[(int)facingDirectionEnemy::E].loop = false;

	basicAttack[(int)facingDirectionEnemy::W].PushBack({ 128, 147, 13, 19 });
	basicAttack[(int)facingDirectionEnemy::W].speed = attackSpeedAnim;
	basicAttack[(int)facingDirectionEnemy::W].loop = false;

	//currentAnimation = &run[(int)facingDirectionEnemy::SE];
	/*SetPivot(8, 32);
	size.create(13,36);*/

	SetPivot(8, 25);
	size.create(16, 28);

	life = 100;
}

EnemyTest::~EnemyTest()
{
	LOG("Bye Enemy Test ");
} 

bool EnemyTest::Start()
{
	
	return true;
}

bool EnemyTest::PreUpdate()
{

	return true;
}

bool EnemyTest::Update(float dt)
{
	/*hitPoint->attachedEntity->name;

	if (hitPoint != nullptr)
	{
		LOG("");
	}*/

	SetState(dt);


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

	if (App->input->GetControllerButton(SDL_CONTROLLER_BUTTON_Y) == KEY_DOWN)
	{
		life -= 30;
		App->HPManager->callHPLabelSpawn(iPoint(this->position.x, this->position.y), 30, ELEMENTAL_TYPE::POISON_ELEMENT);
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
		else if(!App->entityFactory->areAllSubtilesReserved())
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
		fPoint tempPos = GetPivotPos() + velocity * dt * speed;

		/*currentAnimation = &idle[(int)GetPointingDir(atan2f(velocity.y, velocity.x))];
		CheckRenderFlip();*/
		SetLookingTo(currentDestiny.Return_fPoint()); 
		currentAnimation = &run[pointingDir];

		//float current_cycle_frame = currentAnimation->GetCurrentFloatFrame();
		//currentAnimation->SetCurrentFrame(current_cycle_frame);

		// Collision between them currently not working properly
		if (!isNextPosFree(tempPos.ReturniPoint()) && !freePass)
		{	
				state = EnemyState::WAITING;
				checkTime.Start();
				break;
		}
		else position = tempPos - pivot; 

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

		if (path_to_follow.size() <= 0 )
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
		if (checkTime.ReadSec() > attackSpeed && currentAnimation->Finished())
		{
			App->attackManager->AddPropagationAttack(this, GetSubtilePos(), propagationType::BFS, baseDamage, 4, 50);
			basicAttack[pointingDir].Reset();
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
	default:
		break;
	}
}

bool EnemyTest::CleanUp()
{
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

	if (debugSubtile != nullptr)
	{
		App->tex->UnLoad(debugSubtile);
		debugSubtile = nullptr;
	}

	std::list<entityStat*>::iterator item = stat.begin();
	for (; item != stat.end(); ++item)
	{
		stat.remove(*item);
	}
	stat.clear();

	if (entityTex != nullptr)
	{
		App->tex->UnLoad(entityTex);
		entityTex = nullptr;
	}



	return true;
}
