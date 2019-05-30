#include "ChestTrigger.h"
#include "ChestAsset.h"
#include "j1Input.h"

ChestTrigger::ChestTrigger(const fPoint & pos) : Trigger(TRIGGER_TYPE::CHEST, pos.x, pos.y, "ChestTrigger")
{
	nSubtiles = 2; 
	AssignInSubtiles(nSubtiles);
}

ChestTrigger::~ChestTrigger()
{
	DeleteFromSubtiles(nSubtiles);
	myChest = nullptr;
}

bool ChestTrigger::DoTriggerAction()
{
	if (App->input->GetControllerButton(SDL_CONTROLLER_BUTTON_A) == KEY_DOWN || App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	{
		myChest->OpenChest(); 
		to_delete = true; 
	}
	
	return true;
}

void ChestTrigger::AssignOwner(ChestAsset * owner)
{
	myChest = owner; 
}
