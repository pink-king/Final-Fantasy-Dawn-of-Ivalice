#include "EnemyTest.h"
#include "j1Textures.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1Map.h"
#include "j1EntityFactory.h"
#include "PlayerEntity.h"
#include "j1PathFinding.h"
#include "j1LootManager.h"
#include "j1AttackManager.h"
#include "UiItem_HitPointManager.h"
#include "UiItem_Image.h"
#include "j1BuffManager.h"


#include <random>

EnemyTest::EnemyTest(iPoint position, uint speed, uint detectionRange, uint attackRange, float attackSpeed) : Enemy(position, speed, detectionRange, attackRange, attackSpeed)
{
	name.assign("Test");

	// TODO: import from xml
	entityTex = App->tex->Load("textures/enemies/GoblinEnemy.png"); // TODO: note, repetead entities/enemies, load texture once time
	debugSubtile = App->tex->Load("maps/tile_32x32_2.png");

	idle[(int)facingDirectionEnemy::SE].PushBack({ 7,34,13,36 });

	//currentAnimation = &idle[(int)facingDirectionEnemy::SE];
	SetPivot(6, 32);
	size.create(13,36);

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
		App->buff->DirectAttack(App->entityFactory->player->selectedCharacterEntity, this, 30, ELEMENTAL_TYPE::ICE_ELEMENT, "meh");
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
		if (isInDetectionRange() && speed > 0)
		{
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
		else
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
		if (checkTime.ReadSec() > attackSpeed)
		{
			App->attackManager->AddPropagationAttack(this, GetSubtilePos(), propagationType::BFS, 10, 4, 50);
			state = EnemyState::CHECK;
		}	
	}
		break;

	case EnemyState::WAITING:	// Needs a re-planning
	{	
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
