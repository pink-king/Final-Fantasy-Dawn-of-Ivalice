#include <algorithm>
#include "PlayerEntityManager.h"
#include "j1Render.h"
#include "p2Log.h"

PlayerEntityManager::PlayerEntityManager(iPoint position) : j1Entity(NO_TYPE, position.x,position.y, "PEM")
{
	marche = new Marche();
	ritz = new Ritz();
	shara = new Shara();

	characters.push_back(marche);
	characters.push_back(ritz);
	characters.push_back(shara);

	selectedCharacterEntity = marche;
}

PlayerEntityManager::~PlayerEntityManager()
{
	delete marche;
	delete ritz;
	delete shara;

	// TODO: free characters vector
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

	selectedCharacterEntity->Update(dt);

	// update selected character position to its "manager" position
	position = selectedCharacterEntity->position;

	return ret;
}

bool PlayerEntityManager::PostUpdate()
{

	selectedCharacterEntity->PostUpdate();
	
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

	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_DOWN || App->input->GetControllerButton(SDL_CONTROLLER_BUTTON_LEFTSHOULDER) == KEY_DOWN)
	{
		SetPreviousCharacter();
	}

	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_DOWN || App->input->GetControllerButton(SDL_CONTROLLER_BUTTON_RIGHTSHOULDER) == KEY_DOWN)
	{
		SetNextCharacter();
	}

	return ret;
}

void PlayerEntityManager::SetPreviousCharacter()
{
	float current_frame = 0;
	fPoint tempPosition;

	std::vector<PlayerEntity*>::reverse_iterator leftItem = characters.rbegin();

	for (; leftItem != characters.rend(); ++leftItem)
	{
		if ((*leftItem) == selectedCharacterEntity)
		{
			current_frame = selectedCharacterEntity->currentAnimation->GetCurrentFloatFrame();
			tempPosition = selectedCharacterEntity->position;

			++leftItem;
			if (leftItem == characters.rend())
				leftItem = characters.rbegin();

			selectedCharacterEntity = (*leftItem);
			selectedCharacterEntity->currentAnimation->SetCurrentFrame(current_frame);
			selectedCharacterEntity->position = tempPosition;
			selectedCharacterName = selectedCharacterEntity->character;
			// sets current animation
			SetCurrentAnimation();
			break;
		}
	}

	LOG("Selected Character: %c", selectedCharacterEntity->name); // TODO: log crashes with %s, std::string relative
}

void PlayerEntityManager::SetNextCharacter()
{
	float current_frame = 0;
	fPoint tempPosition;

	std::vector<PlayerEntity*>::iterator nextItem = characters.begin();

	for (; nextItem != characters.end(); ++nextItem)
	{
		if ((*nextItem) == selectedCharacterEntity)
		{
			current_frame = selectedCharacterEntity->currentAnimation->GetCurrentFloatFrame();
			tempPosition = selectedCharacterEntity->position;

			++nextItem;
			if (nextItem == characters.end())
				nextItem = characters.begin();
	
			selectedCharacterEntity = (*nextItem);
			selectedCharacterEntity->currentAnimation->SetCurrentFrame(current_frame);
			selectedCharacterEntity->position = tempPosition;
			selectedCharacterName = selectedCharacterEntity->character;
			// sets current animation
			SetCurrentAnimation();
			break;
		}
	}

	LOG("Selected Character: %c", selectedCharacterEntity->name); // TODO: log crashes with %s, std::string relative
}

void PlayerEntityManager::SetCurrentAnimation()
{
	currentAnimation = selectedCharacterEntity->currentAnimation;
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