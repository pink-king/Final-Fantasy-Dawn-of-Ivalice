#include "j1AttackManager.h"
#include "j1EntityFactory.h"
#include "j1Render.h"
#include "p2Log.h"
#include "j1BuffManager.h"
#include "j1Scene.h"
#include <algorithm>

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
	// test propagation attack
	AddPropagationAttack(propagationType::BFS, 10, 6, 20);


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
		if (!(*attacksDataIterator)->Update(dt))
			ret = false;
	}

	return ret;
}

bool j1AttackManager::PostUpdate()
{
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

void j1AttackManager::AddPropagationAttack(propagationType propagationType, int baseDamage, int subTileStepRadius, uint32 propagationStepSpeed)
{
	currentPropagationAttacks.push_back(new attackData(nullptr,propagationType, baseDamage, subTileStepRadius, propagationStepSpeed));
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
		}
	}
}

// ATTACK DATA CLASS -------------------------------------------------------------------------------------------------

attackData::attackData()
{
	Start();
}

attackData::attackData(j1Entity* fromEntity, propagationType type, int baseDamage, int subtileStepRadius, uint32 propagationStepSpeed) :
	 propaType(type), baseDamage(baseDamage), subTileStepRadius(subtileStepRadius), propagationStepSpeed(propagationStepSpeed)
{
	Start();
}

attackData::~attackData()
{}

bool attackData::Start()
{
	stepTimer.Start();

	return true;
}

bool attackData::Update(float dt)
{
	return true;
}

bool attackData::PostUpdate()
{
	return true;
}

std::vector<j1Entity*> attackData::GetInvoldedEntitiesFromSubtile(const iPoint subTile)
{
	std::vector<j1Entity*> temp;
	
	return temp;
}