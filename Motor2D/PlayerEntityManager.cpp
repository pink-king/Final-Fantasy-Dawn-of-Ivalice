#include "PlayerEntityManager.h"
#include "j1Render.h"
#include <algorithm>

PlayerEntityManager::PlayerEntityManager() : j1Entity(NO_TYPE, 0,0, "PEM")
{}

PlayerEntityManager::~PlayerEntityManager()
{
}

//bool PlayerEntityManager::Awake(pugi::xml_node & node)
//{
//	return true;
//}

bool PlayerEntityManager::Start()
{
	return true;
}

bool PlayerEntityManager::PreUpdate()
{
	return true;
}

bool PlayerEntityManager::Update(float dt)
{
	bool ret = true;

	SwapInputChecker(); // checks gamepad triggers input


	return ret;
}

bool PlayerEntityManager::PostUpdate()
{
	return true;
}

bool PlayerEntityManager::CleanUp()
{
	return true;
}

bool PlayerEntityManager::SwapInputChecker()
{
	bool ret = true;

	// checks gamepad and swaps character

	return ret;
}