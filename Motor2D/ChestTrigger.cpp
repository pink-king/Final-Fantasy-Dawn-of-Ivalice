#include "ChestTrigger.h"
#include "ChestAsset.h"
#include "j1EntityFactory.h"
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

void ChestTrigger::Draw()
{
	if (drawA)
	{
		App->render->Blit(App->gui->GetAtlas(), position.x - 5, position.y - 50, &App->input->GetAssociatedRectForThisGamepadInput(
			App->input->gamepadScheme.sharedInput.interact.button,
			App->input->gamepadScheme.sharedInput.interact.axis), 1.0F, SDL_FLIP_NONE, 0.5F);
		drawA = false;
	}
	if (App->scene->debugSubtiles)
		DebugTrigger();
}


bool ChestTrigger::DoTriggerAction()
{
	drawA = true;

	if (App->input->GetControllerGeneralPress(App->input->gamepadScheme.sharedInput.interact) == KEY_DOWN || App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
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
