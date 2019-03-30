#include <algorithm>
#include "PlayerEntityManager.h"
#include "j1Render.h"
#include "p2Log.h"
#include "j1Map.h"
#include "j1BuffManager.h"
//buff test
#include "j1Window.h"

PlayerEntityManager::PlayerEntityManager(iPoint position) : j1Entity(NO_TYPE, position.x,position.y, "PEM")
{
	marche = new Marche(position.x,position.y);
	ritz = new Ritz(position.x, position.y);
	shara = new Shara(position.x, position.y);

	characters.push_back(marche);
	characters.push_back(ritz);
	characters.push_back(shara);

	selectedCharacterEntity = marche;

	// debug normal tile tex
	debugTileTex = App->tex->Load("maps/tile_64x64_2.png");

	debug = true;
	
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
	std::vector<PlayerEntity*>::iterator item = characters.begin();

	for (; item != characters.end(); ++item)
		(*item)->Start();

	pivot = selectedCharacterEntity->pivot;



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
	//provisional function to life
	std::vector<PlayerEntity*>::iterator item = characters.begin();
	for (; item != characters.end(); ++item)
	{
		if ((*item) != selectedCharacterEntity)
			(*item)->life = selectedCharacterEntity->life;
	}

	static char title[30];
	sprintf_s(title, 30, " | player life: %f", (*selectedCharacterEntity).life);
	App->win->SetTitle(title);


	return ret;
}

bool PlayerEntityManager::PostUpdate()
{

	selectedCharacterEntity->PostUpdate();

	if (debug)
	{
		fPoint pivotPos = GetPivotPos();
		iPoint debugTilePos = App->map->WorldToMap(pivotPos.x, pivotPos.y);
		debugTilePos = App->map->MapToWorld(debugTilePos.x, debugTilePos.y);
		App->render->Blit(debugTileTex, debugTilePos.x, debugTilePos.y, NULL);
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

	if (App->input->GetKey(SDL_SCANCODE_KP_4) == KEY_DOWN || App->input->GetControllerButton(SDL_CONTROLLER_BUTTON_LEFTSHOULDER) == KEY_DOWN)
	{
		SetPreviousCharacter();
	}

	if (App->input->GetKey(SDL_SCANCODE_KP_6) == KEY_DOWN || App->input->GetControllerButton(SDL_CONTROLLER_BUTTON_RIGHTSHOULDER) == KEY_DOWN)
	{
		SetNextCharacter();
	}

	return ret;
}

void PlayerEntityManager::SetPreviousCharacter()
{
	float current_frame = 0;
	fPoint tempPosition;
	int pointingDirectionTemp = 0;

	std::vector<PlayerEntity*>::reverse_iterator leftItem = characters.rbegin();

	for (; leftItem != characters.rend(); ++leftItem)
	{
		if ((*leftItem) == selectedCharacterEntity)
		{
			// stores needed data to swap
			current_frame = selectedCharacterEntity->currentAnimation->GetCurrentFloatFrame();
			tempPosition = selectedCharacterEntity->position;
			pointingDirectionTemp = selectedCharacterEntity->pointingDir;

			++leftItem;
			if (leftItem == characters.rend())
				leftItem = characters.rbegin();

			// swaps to
			selectedCharacterEntity = (*leftItem);
			// restores needed data
			selectedCharacterEntity->currentAnimation->SetCurrentFrame(current_frame);
			selectedCharacterEntity->position = tempPosition;
			selectedCharacterEntity->pointingDir = pointingDirectionTemp;
			selectedCharacterName = selectedCharacterEntity->character;
			// sets current animation
			SetCurrentAnimation();
			// updates pivot
			UpdatePivot();
			break;
		}
	}

	LOG("Selected Character: %c", selectedCharacterEntity->name); // TODO: log crashes with %s, std::string relative
}

void PlayerEntityManager::SetNextCharacter()
{
	float current_frame = 0;
	fPoint tempPosition;
	int pointingDirectionTemp = 0;

	std::vector<PlayerEntity*>::iterator nextItem = characters.begin();

	for (; nextItem != characters.end(); ++nextItem)
	{
		if ((*nextItem) == selectedCharacterEntity)
		{
			current_frame = selectedCharacterEntity->currentAnimation->GetCurrentFloatFrame();
			tempPosition = selectedCharacterEntity->position;
			pointingDirectionTemp = selectedCharacterEntity->pointingDir;

			++nextItem;
			if (nextItem == characters.end())
				nextItem = characters.begin();
	
			selectedCharacterEntity = (*nextItem);
			selectedCharacterEntity->currentAnimation->SetCurrentFrame(current_frame);
			selectedCharacterEntity->position = tempPosition;
			selectedCharacterEntity->pointingDir = pointingDirectionTemp;
			selectedCharacterName = selectedCharacterEntity->character;
			// sets current animation
			SetCurrentAnimation();
			// updates pivot
			UpdatePivot();
			break;
		}
	}

	LOG("Selected Character: %c", selectedCharacterEntity->name); // TODO: log crashes with %s, std::string relative
}

void PlayerEntityManager::SetCurrentAnimation()
{
	currentAnimation = selectedCharacterEntity->currentAnimation;
}

void PlayerEntityManager::Draw()
{
	selectedCharacterEntity->Draw();
}

void PlayerEntityManager::UpdatePivot()
{
	pivot = selectedCharacterEntity->pivot;
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