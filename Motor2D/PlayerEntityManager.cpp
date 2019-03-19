#include "PlayerEntityManager.h"
#include "j1Render.h"
#include <algorithm>

PlayerEntityManager::PlayerEntityManager(iPoint position) : j1Entity(NO_TYPE, position.x,position.y, "PEM")
{
	marche = new Marche();
	ritz = new Ritz();
	shara = new Shara();
}

PlayerEntityManager::~PlayerEntityManager()
{
	delete marche;
	delete ritz;
	delete shara;
}

//bool PlayerEntityManager::Awake(pugi::xml_node & node)
//{
//	return true;
//}

bool PlayerEntityManager::Start()
{
	// loads the characters data


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

	switch (selectedCharacter)
	{
	case characterName::MARCHE:
		if (marche != nullptr) 
			marche->Update(dt);
		break;
	case characterName::RITZ:
		if (ritz != nullptr) 
			ritz->Update(dt);
		break;
	case characterName::SHARA:
		if (shara != nullptr) 
			shara->Update(dt);
		break;
	default:
		break;
	}

	return ret;
}

bool PlayerEntityManager::PostUpdate()
{
	switch (selectedCharacter)
	{
	case characterName::MARCHE:
		if (marche != nullptr)
			currentAnimation = marche->currentAnimation;
			Draw(marche->entityTex);
		break;
	/*case characterName::RITZ:
		if (ritz != nullptr)
			ritz->PostUpdate();
		break;
	case characterName::SHARA:
		if (shara != nullptr)
			shara->PostUpdate();
		break;
	default:
		break;*/
	}
	return true;
}

bool PlayerEntityManager::CleanUp()
{
	/*delete marche;
	delete ritz;
	delete shara;*/

	return true;
}

bool PlayerEntityManager::SwapInputChecker()
{
	bool ret = true;

	// checks gamepad and swaps character

	return ret;
}

//bool PlayerEntityManager::Draw()
//{
//	switch (selectedCharacter)
//	{
//	case characterName::MARCHE:
//
//		break;
//	case characterName::RITZ:
//		break;
//	case characterName::SHARA:
//		break;
//	default:
//		break;
//	}
//}