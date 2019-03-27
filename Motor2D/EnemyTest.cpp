#include "EnemyTest.h"
#include "j1Textures.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1Map.h"
#include "j1EntityFactory.h"
#include "PlayerEntity.h"
#include "j1PathFinding.h"

EnemyTest::EnemyTest(iPoint position) : j1Entity(NO_TYPE, position.x, position.y, "PIM")
{
	name.assign("Test");

	// TODO: import from xml
	entityTex = App->tex->Load("textures/enemies/GoblinEnemy.png");

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
	if (SearchNewPath())
	{
		MoveToNextNode(dt);
	}



	return true;
}

bool EnemyTest::PostUpdate()
{
	



	return true;
}

iPoint EnemyTest::GetNextNode()
{
	// get the enemy pos (this position) on map coords. (tile coords.)
	iPoint thisPos;
	thisPos = App->map->WorldToMap((int)position.x, (int)position.y);

	// get the nextNodePos, the last on dyn array (the first pop out) || copylastgenerated path flip the order
	iPoint nextNodePos = path_to_follow.back();

	// compare enemy and nextNode on tile coords, if is the same, pop and get the new nextNode
	iPoint areaPoint = { 1,1 }; // tile values
	if (!(thisPos.x >= (nextNodePos.x + areaPoint.x) || (thisPos.x + 2) <= nextNodePos.x || // enemy tile width 
		thisPos.y >= (nextNodePos.y + areaPoint.y) || (thisPos.y + 2) <= nextNodePos.y)) // enemy tile height
	{
		// Erase the last element on the path
		path_to_follow.pop_back(); 
	}

	if (path_to_follow.size()>0)
		return App->map->MapToWorld(nextNodePos.x, nextNodePos.y);
	else
		return thisPos;

}

bool EnemyTest::SearchNewPath()
{
	bool ret = false;
	iPoint thisPos = App->map->WorldToMap((int)GetPivotPos().x, (int)GetPivotPos().y);
	iPoint playerPos = App->map->WorldToMap((int)App->entityFactory->player->GetPivotPos().x, (int)App->entityFactory->player->GetPivotPos().y);

	if (thisPos.DistanceManhattan(playerPos) > 1) // if the enemy is at more than 1 distance manhattan
	{
		if (App->pathfinding->CreatePath(thisPos, playerPos) > 0)
		{

			path_to_follow = *App->pathfinding->GetLastPath();
			ret = (path_to_follow.size() > 1);
		}
	}

	return ret; 
}

void EnemyTest::MoveToNextNode(float dt)
{
	iPoint nextNode = GetNextNode();

	fPoint velocity_vector;
	velocity_vector.x = (int)nextNode.x;
	velocity_vector.y = (int)nextNode.y;

	velocity_vector -= position;
	velocity_vector.Normalize();

	position += velocity_vector * dt * chasingSpeed;
}




bool EnemyTest::CleanUp()
{

	return true;
}

void EnemyTest::DebugPath() const
{
	for (uint i = 0; i < path_to_follow.size(); ++i)
	{
		iPoint pos = App->map->MapToWorld(path_to_follow[i].x, path_to_follow[i].y);
		App->render->Blit(App->pathfinding->debug_texture, pos.x, pos.y);
	}
	
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

	for (uint i = 0; i < path_to_follow.size(); ++i)
	{
		iPoint pos = App->map->MapToWorld(path_to_follow[i].x, path_to_follow[i].y);
		App->render->Blit(App->pathfinding->debug_texture, pos.x, pos.y);
	}

}
