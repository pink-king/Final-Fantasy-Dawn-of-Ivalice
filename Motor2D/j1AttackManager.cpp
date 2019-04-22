#include "j1AttackManager.h"
#include "j1EntityFactory.h"
#include "j1Render.h"
#include "p2Log.h"
#include "j1BuffManager.h"
#include "j1Scene.h"
#include "j1PathFinding.h"
#include "j1Map.h"
#include "j1ParticlesClassic.h"
#include "Brofiler/Brofiler.h"

// TODO: to improve ---
// - unlink the speed of the steps from time, do with float increments multiplicative with dt

j1AttackManager::j1AttackManager()
{
	name.assign("AttackManager");

	matrix[PLAYER][PLAYER] = false;
	matrix[PLAYER][ENEMY_BOMB] = true;
	matrix[PLAYER][ENEMY_TEST] = true;

	matrix[ENEMY_BOMB][ENEMY_BOMB] = false;
	matrix[ENEMY_BOMB][PLAYER] = true;
	matrix[ENEMY_BOMB][ENEMY_TEST] = false;

	matrix[ENEMY_TEST][PLAYER] = true;
	matrix[ENEMY_TEST][ENEMY_BOMB] = false;
	matrix[ENEMY_TEST][ENEMY_TEST] = false;


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
	BROFILER_CATEGORY("Attacks Propagations", Profiler::Color::Chocolate);

	bool ret = true;

	// check if we have any attack queued to instantiate
	/*while (!queuedAttacks.empty())
	{
		AddPropagationAttack(queuedAttacks.front());
		queuedAttacks.pop();
	}*/

	std::vector<attackData*>::iterator attacksDataIterator = currentPropagationAttacks.begin();

	for (; attacksDataIterator != currentPropagationAttacks.end();)
	{
		if (!(*attacksDataIterator)->to_erase)
		{
			if (!(*attacksDataIterator)->Update(dt))
				ret = false;

			++attacksDataIterator;
		}
		else
		{
			LOG("Involved %i entities", (*attacksDataIterator)->combo);
			delete(*attacksDataIterator);
			(*attacksDataIterator) = nullptr;
			attacksDataIterator = currentPropagationAttacks.erase(attacksDataIterator);
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

void j1AttackManager::AddPropagationAttack(const j1Entity* fromEntity, iPoint startSubtilePoint,propagationType propagationType, damageType dmgType, ELEMENTAL_TYPE elemType, int baseDamage, int subTileStepRadius, uint32 propagationStepSpeed, bool instantiateParticles, bool paralize)
{
	currentPropagationAttacks.push_back(new attackData(fromEntity, startSubtilePoint,propagationType, dmgType, elemType, baseDamage, subTileStepRadius, propagationStepSpeed, instantiateParticles));
}

//void j1AttackManager::AddPropagationAttack(attackData* data)
//{
//	currentPropagationAttacks.push_back(data);
//}
//
//void j1AttackManager::AddPropagationAttackToQueue(attackData* data)
//{
//	queuedAttacks.push(data);
//}

// ATTACK DATA CLASS -------------------------------------------------------------------------------------------------

attackData::attackData()
{
	//Start();
}

attackData::attackData(const j1Entity* fromEntity,iPoint startSubtilePoint, propagationType type, damageType dmgType, ELEMENTAL_TYPE elemType, int baseDamage, int subtileStepRadius, uint32 propagationStepSpeed, bool instantiateParticles,bool paralize) :
	 fromEntity(fromEntity), startSubtilePoint(startSubtilePoint) ,propaType(type), dmgType(dmgType), elemType(elemType),
	baseDamage(baseDamage),subTileStepRadius(subtileStepRadius), propagationStepSpeed(propagationStepSpeed), instantiateParticles(instantiateParticles),paralize(paralize)
{
	//fromType = fromEntity->type;
	Start();
}

attackData::~attackData()
{
	LOG("destroyed attackdata package");
}

bool attackData::Start()
{
	//debug = false;
	// check detection resolution
	//propagationResolution = PROPAGATION_RESOLUTION;
	// -----
	stepTimer.Start();
	// puts startpoint to algoritm
	frontier.push(startSubtilePoint);
	visited.push_back(startSubtilePoint);
	// " and do the first propagation step "
	subtileQueue.push(startSubtilePoint);
	CheckEntitiesFromSubtileStep();
	DoAttackType();

	if (instantiateParticles)
	{
		lastStepSubtiles.push(startSubtilePoint);
		InstantiateParticles();
	}
	

	return true;
}

bool  attackData::DoAttackType()
{
	bool ret = true;

	switch (dmgType)
	{
	case damageType::NO:
		break;
	case damageType::DIRECT:
		DoDirectAttack();
		break;
	case damageType::INTIME:
		DoInTimeAttack();
		break;
	case damageType::MAX:
		break;
	default:
		break;
	}

	return ret;
}

bool attackData::Update(float dt)
{
	if (stepTimer.Read() > propagationStepSpeed)
	{
		//LOG("DO next propagation step");
		stepTimer.Start();
		if (currentPropagationStep >= subTileStepRadius - 1 ) // -1 because how frontier works
		{
			to_erase = true;
		}
		else
		{
			// propagate
			DoNextPropagationStep();
			// check queued subtiles to find target entities
			CheckEntitiesFromSubtileStep(); // checks an adds filtered entities to final queue to communicate with buff manager
			DoAttackType(); // attack all entities involved on last frame update on the affected step subtiles
		}
		
		//InstatiateReplica();
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
	// Draw particles if we want

	if (instantiateParticles)
		InstantiateParticles();
	

	return true;
}

bool attackData::InstantiateParticles()
{
	bool ret = true;

	if (instantiateParticles)
	{
		while (!lastStepSubtiles.empty())
		{
			iPoint drawPosition = lastStepSubtiles.front();
			lastStepSubtiles.pop();

			InstantiateParticleType(App->map->SubTileMapToWorld(drawPosition.x, drawPosition.y));
		}
	}
	else
		ret = false;

		return ret;
}

bool attackData::InstantiateParticleType(iPoint drawPos) // TODO: maybe pass directly the particle to attack manager instead of the type
{
	bool ret = true;

	// center particle to subtile
	iPoint subtileSize = { 32,16 };
	iPoint drawRectified;
	// center at subtile ------
	drawRectified.x = drawPos.x + subtileSize.x * 0.5f;
	drawRectified.y = drawPos.y + (subtileSize.y * 0.5f) * 2;

	// flip particles pseudo randomly
	SDL_RendererFlip renderFlip = SDL_RendererFlip::SDL_FLIP_NONE;

	if (rand() % 2 == 0)
	{
		renderFlip = SDL_RendererFlip::SDL_FLIP_HORIZONTAL;
	}
	// ----------------------------------

	switch (elemType)
	{
	case ELEMENTAL_TYPE::NO_ELEMENT:
		break;
	case ELEMENTAL_TYPE::FIRE_ELEMENT:
	{
		iPoint firePivot = { 8,46 };
		// ------------------------
		// substract pivot
		drawRectified -= firePivot;
		App->particles->AddParticle(App->particles->fire01, drawRectified.x, drawRectified.y , { 0,0 }, 0u, renderFlip); 
		iPoint smokePivot = { 11, -17 };
		drawRectified -= smokePivot;
		App->particles->AddParticle(App->particles->smoke01, drawRectified.x, drawRectified.y, { 0,0 }, 800u, renderFlip);
		break;
	}
	case ELEMENTAL_TYPE::ICE_ELEMENT:
	{
		iPoint ice03Pivot = { 8, 48 };
		// ------------------------
		// substract pivot
		drawRectified -= ice03Pivot;
		App->particles->AddParticle(App->particles->ice03, drawRectified.x, drawRectified.y, { 0,0 }, 0u, renderFlip);
		break;
	}
	case ELEMENTAL_TYPE::POISON_ELEMENT:
	{
		iPoint poison01Pivot = { 8, 8 };
		// ------------------------
		// substract pivot
		drawRectified -= poison01Pivot;
		if(rand() % 2 == 0)
		{
			App->particles->AddParticle(App->particles->poison01, drawRectified.x, drawRectified.y, { 0,0 }, 0u, renderFlip);
			App->particles->AddParticle(App->particles->poison02, drawRectified.x, drawRectified.y, { 0,0 }, 100u, renderFlip);
		}
		else
		{
			App->particles->AddParticle(App->particles->poison02, drawRectified.x, drawRectified.y, { 0,0 }, 0u, renderFlip);
			App->particles->AddParticle(App->particles->poison01, drawRectified.x, drawRectified.y, { 0,0 }, 100u, renderFlip);
		}
		break;
	}
	case ELEMENTAL_TYPE::ALL_ELEMENTS:
		break;
	case ELEMENTAL_TYPE::MAX:
		break;
	default:
		break;
	}

	return true;
}

//bool attackData::InstatiateReplica()
//{
//	if (propagationResolution > 1)
//	{
//		attackData* newAttackData = new attackData();
//		*newAttackData = *this;
//		newAttackData->propagationResolution--;
//		this->propagationResolution--;
//		
//		App->attackManager->AddPropagationAttackToQueue(newAttackData);
//	}
//
//	return true;
//}

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

std::vector<j1Entity*>* attackData::GetInvolvedEntitiesFromSubtile(const iPoint subTile)
{
	std::vector<j1Entity*>* temp;

	temp = App->entityFactory->GetSubtileEntityVectorAt(subTile);

	if (temp != nullptr)
	{
		if (temp->size() > 0)
		{
			LOG("vector with: %i entity/ies", temp->size());
			return temp;
		}
	}
	
	return nullptr;
}

void attackData::CheckEntitiesFromSubtileStep()
{
	while (!subtileQueue.empty())
	{
		//LOG("Checking entities on involved subtile");
		iPoint subtileChecker = subtileQueue.front();
		subtileQueue.pop();
		
	    std::vector<j1Entity*>* involvedEntities = GetInvolvedEntitiesFromSubtile(subtileChecker);
		if (involvedEntities != nullptr)
		{
			std::vector<j1Entity*>::iterator invoterator = involvedEntities->begin();
			for (; invoterator != involvedEntities->end(); ++invoterator)
			{
				LOG("Entity name %s", (*invoterator)->name.begin());

				// filter an add the target entities to final queue
				AddEntityToQueueFiltered((*invoterator));
			}
		}
		
	}
}

bool attackData::AddEntityToQueueFiltered(j1Entity* entityToFilter)
{
	if (fromEntity != nullptr)
	{
		if (App->attackManager->matrix[fromEntity->type][entityToFilter->type])
		{
			entitiesQueue.push(entityToFilter);
		}

		return true;
	}

	return false;
}

bool attackData::DoDirectAttack()
{
	while (!entitiesQueue.empty()) // if we have any valid entity on last propagation step
	{
		// pass the type etc of the attack, for now pass a direct attack
		j1Entity* defender = entitiesQueue.front();
		entitiesQueue.pop();
		if (fromEntity == nullptr)
			LOG("");
		App->buff->DirectAttack((j1Entity*)fromEntity, defender, baseDamage, elemType,"obsolete?");

		// updates combo counter
		++combo;
	}
	return true;
}

bool attackData::DoInTimeAttack()
{
	bool ret = true;

	float totalTimeMultiplier = 0.04f;

	while (!entitiesQueue.empty()) // if we have any valid entity on last propagation step
	{
		// pass the type etc of the attack, for now pass a direct attack
		j1Entity* defender = entitiesQueue.front();
		entitiesQueue.pop();
		
		switch (elemType)
		{
		case ELEMENTAL_TYPE::NO_ELEMENT:
			LOG("wtf, if no element, no party");
			break;
		case ELEMENTAL_TYPE::FIRE_ELEMENT:
			App->buff->CreateBurned((j1Entity*)fromEntity, defender, (float)baseDamage, propagationStepSpeed * totalTimeMultiplier, "fuckYouState", paralize);
			break;
		case ELEMENTAL_TYPE::ICE_ELEMENT: // WARNING: VERY OP
			App->buff->CreateParalize((j1Entity*)fromEntity, defender, (float)baseDamage, propagationStepSpeed * totalTimeMultiplier, "fuckYouState", paralize);
			break;
		case ELEMENTAL_TYPE::POISON_ELEMENT:
			App->buff->CreatePoision((j1Entity*)fromEntity, defender, (float)baseDamage, propagationStepSpeed * totalTimeMultiplier, "fuckYouState", paralize);
			break;
		case ELEMENTAL_TYPE::ALL_ELEMENTS:
			App->buff->CreateBurned((j1Entity*)fromEntity, defender, (float)baseDamage, propagationStepSpeed * totalTimeMultiplier, "fuckYouState", paralize);
			App->buff->CreatePoision((j1Entity*)fromEntity, defender, (float)baseDamage, propagationStepSpeed * totalTimeMultiplier, "fuckYouState", paralize);
			App->buff->CreateParalize((j1Entity*)fromEntity, defender, (float)baseDamage, propagationStepSpeed * totalTimeMultiplier, "fuckYouState", paralize);
			break;
		case ELEMENTAL_TYPE::MAX:
			break;
		default:
			break;
		}

		// updates combo counter
		++combo;
	}

	return ret;
}

bool attackData::DoNextStepBFS()
{

	int frontierRadius = 1; // test for twice step (more resolution, but less "wave effect")

	if (!frontier.empty())
	{
		for (int z = 0; z < frontierRadius; ++z)
		{
			int steps = frontier.size();
			for (int i = 0; i < steps; ++i)
			{
				//LOG("DOING: %i", i);
				iPoint currentSubtile = frontier.front();
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
							// adds all subtile affected if we want particle instantiation on that positions
							if (instantiateParticles)
								lastStepSubtiles.push(neighbours[i]);
						}
					}
				}
			}
			// increase step
			++currentPropagationStep;
			/*LOG("current propagation step:%i", currentPropagationStep);
			LOG("frontier size: %i", frontier.size());*/
		}
	}
	else
		to_erase = true; // invalid start position eraser

	return true;
}

const j1Entity* attackData::GetLinkedEntity() const
{
	return fromEntity;
}

const void attackData::UnlinkFromEntity() // unlink and deletes
{
	fromEntity = nullptr;
	to_erase = true;
}

bool j1AttackManager::DestroyAllMyCurrentAttacks(const j1Entity* me) const // one enemy may can have multiple instantiated propagation attacks
{
	bool ret = false;

	std::vector<attackData*>::const_iterator currentAttacksItr = currentPropagationAttacks.cbegin();
	
	uint numCurrentAttacks = 0;

	for (; currentAttacksItr != currentPropagationAttacks.end(); ++currentAttacksItr)
	{
		if ((*currentAttacksItr)->GetLinkedEntity() == me)
		{
			(*currentAttacksItr)->UnlinkFromEntity();
			//LOG("unliked attack from attacker");
			++numCurrentAttacks;
			ret = true;
		}
	}

	LOG("unliked %i attacks", numCurrentAttacks);

	return ret;
}