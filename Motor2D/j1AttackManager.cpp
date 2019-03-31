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
	return true;
}

bool j1AttackManager::PreUpdate()
{
	return true;
}

bool j1AttackManager::Update(float dt)
{
	return true;
}

bool j1AttackManager::PostUpdate()
{
	return true;
}

bool j1AttackManager::CleanUp()
{
	return true;
}