#include <algorithm>
#include "j1AttackManager.h"
#include "j1EntityFactory.h"
#include "j1Render.h"
#include "p2Log.h"
#include "j1BuffManager.h"
#include "j1Scene.h"
#include "j1PathFinding.h"
#include "j1Map.h"

// TODO: to improve ---
// - unlink the speed of the steps from time, do with float increments multiplicative with dt

j1AttackManager::j1AttackManager()
{
	name.assign("AttackManager");
}

j1AttackManager::~j1AttackManager()
{
}

bool j1AttackManager::Awake(pugi::xml_node & node)
{
	return true;
}

bool j1AttackManager::Start()
{
	debugSubtileTex = App->tex->Load("maps/subtileAttackDebug.png");
	debugSubtileTex2 = App->tex->Load("maps/tile_32x32.png");
	debugSubtileTex3 = App->tex->Load("maps/entitiesInvolvedSubtile.png");

	return true;
}

bool j1AttackManager::PreUpdate()
{
	return true;
}

bool j1AttackManager::Update(float dt)
{
	bool ret = true;

	std::vector<attackData*>::iterator attacksDataIterator = currentPropagationAttacks.begin();

	for (; attacksDataIterator != currentPropagationAttacks.end(); ++attacksDataIterator)
	{
		if (!(*attacksDataIterator)->to_erase)
		{
			if (!(*attacksDataIterator)->Update(dt))
				ret = false;
		}
		else
		{
			RemovePropagationAttack(*attacksDataIterator);
			--attacksDataIterator;
		}
	}

	return ret;
}

bool j1AttackManager::PostUpdate()
{
	bool ret = true;

	std::vector<attackData*>::iterator attacksDataIterator = currentPropagationAttacks.begin();

	for (; attacksDataIterator != currentPropagationAttacks.end(); ++attacksDataIterator)
	{
		if ((*attacksDataIterator)->PostUpdate())
			ret = false;
		
	}
	return true;
}

bool j1AttackManager::CleanUp()
{
	if (currentPropagationAttacks.size() > 0)
	{
		LOG("TODO");
	}

	return true;
}

void j1AttackManager::AddPropagationAttack(iPoint startSubtilePoint,propagationType propagationType, int baseDamage, int subTileStepRadius, uint32 propagationStepSpeed)
{
	currentPropagationAttacks.push_back(new attackData(nullptr, startSubtilePoint,propagationType, baseDamage, subTileStepRadius, propagationStepSpeed));
}

void j1AttackManager::RemovePropagationAttack(attackData* attackDataPackage)
{
	std::vector<attackData*>::iterator currentAttacksIterator = currentPropagationAttacks.begin();

	for (; currentAttacksIterator != currentPropagationAttacks.end(); ++currentAttacksIterator)
	{
		if ((*currentAttacksIterator) == attackDataPackage)
		{
			currentPropagationAttacks.erase(currentAttacksIterator);
			LOG("succesfully removed propagationattack");
			break;
		}
	}
}

// ATTACK DATA CLASS -------------------------------------------------------------------------------------------------

attackData::attackData()
{
	Start();
}

attackData::attackData(j1Entity* fromEntity,iPoint startSubtilePoint, propagationType type, int baseDamage, int subtileStepRadius, uint32 propagationStepSpeed) :
	 startSubtilePoint(startSubtilePoint) ,propaType(type), baseDamage(baseDamage), subTileStepRadius(subtileStepRadius), propagationStepSpeed(propagationStepSpeed)
{
	Start();
}

attackData::~attackData()
{}

bool attackData::Start()
{
	stepTimer.Start();

	// puts startpoint to algoritm
	frontier.push(startSubtilePoint);
	visited.push_back(startSubtilePoint);
	// " and do the first propagation step "
	subtileQueue.push(startSubtilePoint);
	CheckEntitiesFromSubtileStep();
	//DoNextPropagationStep();

	return true;
}

bool attackData::Update(float dt)
{
	if (stepTimer.Read() > propagationStepSpeed)
	{
		//LOG("DO next propagation step");
		stepTimer.Start();
		if (currentPropagationStep >= subTileStepRadius - 1) // -1 because how frontier works
		{
			to_erase = true;
		}
		else
		{
			// propagate
			DoNextPropagationStep();
			// check queued subtiles to find target entities
			CheckEntitiesFromSubtileStep();
		}
	}

	// debug draw
	if (debug)
	{
		// blit debug attack expansion -----------------------------------------------------------------
		std::vector<iPoint>::iterator debugDrawVisitedSubtiles = visited.begin();
		for (; debugDrawVisitedSubtiles != visited.end(); ++debugDrawVisitedSubtiles)
		{
			iPoint drawPosition = App->map->SubTileMapToWorld((*debugDrawVisitedSubtiles).x, (*debugDrawVisitedSubtiles).y);
			App->render->Blit(App->attackManager->debugSubtileTex, drawPosition.x, drawPosition.y, NULL);
		}

		std::queue<iPoint> tempQueue = frontier;
		while (!tempQueue.empty())
		{
			iPoint drawPosition = tempQueue.front();
			tempQueue.pop();
			drawPosition = App->map->SubTileMapToWorld(drawPosition.x, drawPosition.y);
			App->render->Blit(App->attackManager->debugSubtileTex2, drawPosition.x, drawPosition.y, NULL);
		}

		/*std::queue<iPoint> subtileQueueTemp = subtileQueue;
		while (!subtileQueueTemp.empty())
		{
			iPoint drawPosition = subtileQueueTemp.front();
			subtileQueueTemp.pop();
			drawPosition = App->map->SubTileMapToWorld(drawPosition.x, drawPosition.y);
			App->render->Blit(App->attackManager->debugSubtileTex3, drawPosition.x, drawPosition.y, NULL);
		}*/
		// -----------------------------------------------------------------------------------------------
	}

	return true;
}

bool attackData::PostUpdate()
{
	return true;
}

bool attackData::DoNextPropagationStep()
{
	switch (propaType)
	{
	case propagationType::BFS:
		DoNextStepBFS();
		break;
	case propagationType::HEURISTIC:
		//DoNextStepHeuristic();
		break;
	default:
		break;
	}
	return true;
}

std::vector<j1Entity*>* attackData::GetInvoldedEntitiesFromSubtile(const iPoint subTile)
{
	std::vector<j1Entity*>* temp;

	temp = App->entityFactory->GetSubtileEntityVectorAt(subTile);

	if (temp != nullptr)
	{
		if (temp->size() > 0)
			LOG("vector with at least one entity");
	}
	
	return temp;
}

void attackData::CheckEntitiesFromSubtileStep()
{
	while (!subtileQueue.empty()) // TODO: get identifier to filter target entities of this attack
	{
		//LOG("Checking entities on involved subtile");
		iPoint subtileChecker = subtileQueue.front();
		subtileQueue.pop();
		
		GetInvoldedEntitiesFromSubtile(subtileChecker);
	}
}

bool attackData::DoNextStepBFS()
{
	//int numNeighbours = 4; // up,down,left,right
	if (!frontier.empty())
	{
		int steps = frontier.size();
		for (int i = 0; i < steps; ++i)
		{
			//LOG("DOING: %i", i);
			iPoint currentSubtile = frontier.front();//front();
			frontier.pop(); // pops last queue value

			// each relative subtile neighbour
			iPoint neighbours[4];
			neighbours[0] = { currentSubtile.x, currentSubtile.y - 1 }; // N
			neighbours[1] = { currentSubtile.x + 1, currentSubtile.y }; // E
			neighbours[2] = { currentSubtile.x, currentSubtile.y + 1 }; // S
			neighbours[3] = { currentSubtile.x - 1, currentSubtile.y }; // W

			for (int i = 0; i < 4; ++i)
			{
				if (std::find(visited.begin(), visited.end(), neighbours[i]) != visited.end())
					continue;
				else
				{
					// check subtile walkability
					iPoint subtileWalkabilityCheck = neighbours[i]; // TODO: we need a direct conversor function
					subtileWalkabilityCheck = App->map->SubTileMapToWorld(subtileWalkabilityCheck.x, subtileWalkabilityCheck.y);
					subtileWalkabilityCheck = App->map->WorldToMap(subtileWalkabilityCheck.x, subtileWalkabilityCheck.y);
					if (App->pathfinding->IsWalkable(subtileWalkabilityCheck))
					{
						frontier.push(neighbours[i]);
						visited.push_back(neighbours[i]);
						// adds to subtile queue too
						subtileQueue.push(neighbours[i]);
					}
				}
			}
		}
		// increase step
		++currentPropagationStep;
		/*LOG("current propagation step:%i", currentPropagationStep);
		LOG("frontier size: %i", frontier.size());*/
	}
	else
		to_erase = true; // invalid start position eraser

	return true;
}