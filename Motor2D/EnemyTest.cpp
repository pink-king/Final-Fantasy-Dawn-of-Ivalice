#include "EnemyTest.h"
#include "j1Textures.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1Map.h"
#include "j1EntityFactory.h"
#include "PlayerEntity.h"
#include "j1PathFinding.h"

EnemyTest::EnemyTest(iPoint position) : j1Entity(ENEMY_TEST, position.x, position.y, "ENEMY_TEST")
{
	name.assign("Test");

	// TODO: import from xml
	entityTex = App->tex->Load("textures/enemies/GoblinEnemy.png");
	debugSubtile = App->tex->Load("maps/tile_32x32_2.png");

	idle.PushBack({ 7,34,13,36 });

	currentAnimation = &idle;
	SetPivot(6, 32);
	size.create(13,36);
}

EnemyTest::~EnemyTest()
{
	//if (collider.collider != nullptr)
	//	collider.collider->to_delete = true;
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
		fPoint p_position = App->entityFactory->player->position;

		// translate to map coords
		iPoint thisPos = App->map->WorldToMap((int)position.x, (int)position.y);
		iPoint playerPos = App->map->WorldToMap((int)p_position.x, (int)p_position.y);

		//if (thisPos.DistanceManhattan(playerPos) < RANGE /*&& App->input->GetKey(SDL_SCANCODE_F) == KEY_DOWN*/)
		//	state = EnemyState::SEARCHPATH; 
	}
		break; 

	case EnemyState::WAITING:
		if (checkTime.ReadSec() < 3)
		{
			if (isNextSubtileFree(newPosition.x, newPosition.y))
				state = EnemyState::GO_NEXT_TILE;
		}
		else
		{
			state = EnemyState::IDLE;
		}
		break;

	case EnemyState::SEARCHPATH:
	{
		if (SearchNewPath())
			state = EnemyState::GET_NEXT_TILE;
		else state = EnemyState::IDLE;

	}
		break;

	case EnemyState::GET_NEXT_TILE:
	{
		if (path_to_follow.size() == 0)
			state = EnemyState::IDLE;
		else
		{	
		tileToGo = path_to_follow.front(); // Get the first element of the list
		state = EnemyState::GO_NEXT_TILE;
		}

		break;
	}

	case EnemyState::GO_NEXT_TILE:
	{
		iPoint toGo = App->map->MapToWorld(tileToGo.x + 1, tileToGo.y);			// Places the next node correctly (1 row below)
		fPoint toGoCenter = { (float)toGo.x, (float)toGo.y + App->map->data.tile_height * 0.5F};	// Center of the tile 
		App->render->DrawCircle(toGoCenter.x, toGoCenter.y, 5, 255, 0, 0, 255, false);

		velocity = toGoCenter - GetPivotPos();
		velocity.Normalize(); 
		position +=  velocity * dt * 100;
		
		// IN PROGRESS
		/*if (imOnSubtile == previousSubtilePos)
		{
			if (!isNextSubtileFree(GetPivotPos().x, GetPivotPos().y))
			{
				state = EnemyState::WAITING;
				checkTime.Start();
				LOG("Next subtile occupied -  WAITING");
			}
		}*/

		if (GetPivotPos().DistanceTo(toGoCenter) < 5 )
		{
			path_to_follow.erase(path_to_follow.begin());
			state = EnemyState::GET_NEXT_TILE;
		}
	}
		break;

	default:
		break; 
	}
}

bool EnemyTest::SearchNewPath()
{
	bool ret = false;
	path_to_follow.clear(); 
	iPoint thisTile = App->map->WorldToMap((int)GetPivotPos().x, (int)GetPivotPos().y);
	iPoint playerTile = App->map->WorldToMap((int)App->entityFactory->player->GetPivotPos().x, (int)App->entityFactory->player->GetPivotPos().y);

	if (thisTile.DistanceManhattan(playerTile) > 1) // The enemy doesnt collapse with the player
	{
		if (App->pathfinding->CreatePath(thisTile, playerTile) != -1)
		{
			path_to_follow = *App->pathfinding->GetLastPath();
			path_to_follow.erase(path_to_follow.begin());		// Enemy doesnt go to the center of his initial tile
			//path_to_follow.pop_back();							// Enemy doesnt eat the player, stays at 1 tile
			ret = (path_to_follow.size() > 0);
		}
		else LOG("Could not create path correctly");
	}

	return ret; 
}

bool EnemyTest::isNextSubtileFree(int x, int y) const
{
	iPoint subTile = App->map->WorldToSubtileMap(x, y);
	return App->entityFactory->isThisSubtileEmpty(subTile);
}

bool EnemyTest::CleanUp()
{
	if (debugSubtile != nullptr)
	{
		App->tex->UnLoad(debugSubtile);
		debugSubtile = nullptr;
	}

	path_to_follow.clear();
	return true;
}

void EnemyTest::DebugPath() const
{
	//for (uint i = 0; i < path_to_follow.size(); ++i)
	//{
	//	iPoint pos = App->map->MapToWorld(path_to_follow[i].x, path_to_follow[i].y);
	//	App->render->DrawCircle(pos.x, pos.y + App->map->data.tile_height * 0.5F, 10, 0, 255, 255, 255, false);
	//	App->render->Blit(App->pathfinding->debug_texture, pos.x, pos.y);
	//}
	//
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
