#include "EnemyTest.h"
#include "j1Textures.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1Map.h"
#include "j1EntityFactory.h"
#include "PlayerEntity.h"
#include "j1PathFinding.h"
#include "j1LootManager.h"

#include <random>

EnemyTest::EnemyTest(iPoint position) : j1Entity(ENEMY_TEST, position.x, position.y, "ENEMY_TEST")
{
	name.assign("Test");

	// TODO: import from xml
	entityTex = App->tex->Load("textures/enemies/GoblinEnemy.png"); // TODO: note, repetead entities/enemies, load texture once time
	debugSubtile = App->tex->Load("maps/tile_32x32_2.png");

	idle.PushBack({ 7,34,13,36 });

	currentAnimation = &idle;
	SetPivot(6, 32);
	size.create(13,36);

	life = 100;
}

EnemyTest::~EnemyTest()
{

	iPoint enemypos = { App->lootManager->GetEnemySubtile(this).x, App->lootManager->GetEnemySubtile(this).y };



	App->lootManager->CreateLoot(App->lootManager->SetLootPos(enemypos.x,enemypos.y).x, App->lootManager->SetLootPos(enemypos.x, enemypos.y).y, "no");
	/*LOG("LOOtValue %iX %iY", App->loot->loot_pos.x, App->loot->loot_pos.y);*/
	LOG("Bye Enemy Test %fx %fy ", position.x, position.y);
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

	SetState(dt);


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
		if (GetPivotPos().DistanceManhattan(App->entityFactory->player->GetPivotPos()) > RANGE)
		{
			state = EnemyState::SEARCHPATH;
		}
		// else go back to spawn point?

	}
	break;
	case EnemyState::SEARCHPATH:
	{
		if (!isMeleeRange)
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
		if (!isMeleeRange)
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

		// Collision between them currently not working properly

		//iPoint onSubtilePosTemp = App->map->WorldToSubtileMap(GetPivotPos().x, GetPivotPos().y);
		//if (onSubtilePosTemp != previousSubtilePos)
		//{
		//	if (!App->entityFactory->isThisSubtileEmpty(onSubtilePosTemp) && !freePass /*&& !isMeleeRange*/)
		//	{
		//		position -= velocity * dt *speed;
		//		state = EnemyState::WAITING;
		//		checkTime.Start();
		//		break;
		//	}
		//}
		state = EnemyState::CHECK;
	}
	break;

	case EnemyState::CHECK:
	{

		if (App->entityFactory->player->ChangedTile())
		{
			App->entityFactory->ReleaseAllReservedSubtiles();
			if(checkTime.Read() > GetRandomValue(250, 1000))
				state = EnemyState::SEARCHPATH;
		}
		else if (GetPivotPos().DistanceTo(currentDestiny.Return_fPoint()) < 5)
		{
			if (path_to_follow.size() == 1 && isMeleeRange)
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
			if (GetSubtilePos().DistanceTo(App->entityFactory->player->GetSubtilePos()) <= 1)
			{
				state = EnemyState::ATTACK;
				LOG("Attacking!");
				checkTime.Start();
			}
			else state = EnemyState::SEARCHPATH;
		}
		

		// Changes range conditions

		if (GetTilePos().DistanceManhattan(App->entityFactory->player->GetTilePos()) < 3)
		{
			if (!isMeleeRange)
				state = EnemyState::SEARCHPATH;
			isMeleeRange = true;
		}
		else
		{
			if (isMeleeRange)
				state = EnemyState::SEARCHPATH;
			isMeleeRange = false;
		}
		break;
	}
	case EnemyState::ATTACK:
	{
		if (checkTime.ReadSec() > 1)
			state = EnemyState::CHECK;
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
		if (cont > 50)
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



bool EnemyTest::SearchNewPath()
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
			
			/*if (path_to_follow.size() > 0)
				path_to_follow.pop_back();*/							// Enemy doesnt eat the player, stays at 1 tile
			//path_to_follow.pop_back();
			ret = (path_to_follow.size() > 0);
		}
		//else LOG("Could not create path correctly");
	}

	return ret; 
}

bool EnemyTest::SearchNewSubPath()
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
			
			if(path_to_follow.size() > 0)
				path_to_follow.pop_back();							// Enemy doesnt eat the player, stays at 1 tile
			
			iPoint adj = path_to_follow.back();
			App->entityFactory->ReserveAdjacent(adj);
			ret = (path_to_follow.size() > 0);
		}
		else LOG("Could not create path correctly");
	}

	return ret;
}

bool EnemyTest::CheckDistance()
{
	return (GetTilePos().DistanceManhattan(App->entityFactory->player->GetTilePos()) < 3);
}



int EnemyTest::GetRandomValue(const int& min,  const int& max)
{
	static std::default_random_engine generator;
	std::uniform_int_distribution<int> distribution(min, max);
	float dice_roll = distribution(generator);
	return dice_roll; 
}

bool EnemyTest::CleanUp()
{
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

	std::vector<items*>::iterator iter = bagObjects.begin();
	for (; iter != bagObjects.end(); ++iter)
	{
		delete *iter;
		*iter = nullptr;
	}
	std::vector<items*>::iterator iter2 = equipedObjects.begin();
	for (; iter2 != equipedObjects.end(); ++iter2)
	{
		delete *iter2;
		*iter2 = nullptr;
	}

	return true;
}

void EnemyTest::DebugPath() const
{
	for (uint i = 0; i < path_to_follow.size(); ++i)
	{
		if (!isMeleeRange) {
			iPoint pos = App->map->MapToWorld(path_to_follow[i].x + 1, path_to_follow[i].y);		// X + 1, Same problem with map
			App->render->DrawCircle(pos.x, pos.y + App->map->data.tile_height * 0.5F, 10, 0, 255, 255, 255, false);
			//App->render->Blit(App->pathfinding->debug_texture, pos.x, pos.y);
		}
		else
		{
			iPoint pos = App->map->SubTileMapToWorld(path_to_follow[i].x + 1, path_to_follow[i].y);		// X + 1, Same problem with map
			App->render->DrawCircle(pos.x, pos.y + App->map->data.tile_height * 0.5F * 0.5F, 5, 0, 255, 0, 255, false);
			//App->render->Blit(App->pathfinding->debug_texture, pos.x, pos.y);
		}
		
	}
	
	iPoint subTilePos = GetSubtilePos();
	subTilePos = App->map->SubTileMapToWorld(subTilePos.x, subTilePos.y);
	App->render->Blit(debugSubtile, subTilePos.x, subTilePos.y, NULL);
}


void EnemyTest::Draw()
{
	if (entityTex != nullptr)
	{
		if (currentAnimation != nullptr)
			App->render->Blit(entityTex, position.x, position.y, &currentAnimation->GetCurrentFrame(), 1.0F, flip);
		else
			App->render->Blit(entityTex, position.x, position.y);
	}

	DebugPath();
}
