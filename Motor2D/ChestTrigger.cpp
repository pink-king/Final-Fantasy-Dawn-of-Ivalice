#include "ChestTrigger.h"
#include "ChestAsset.h"
#include "j1EntityFactory.h"
#include "j1Input.h"

ChestTrigger::ChestTrigger(const fPoint & pos) : Trigger(TRIGGER_TYPE::CHEST, pos.x, pos.y, "ChestTrigger")
{
	nSubtiles = 2; 
	AssignInSubtiles(nSubtiles);

	entityTex = App->entityFactory->ButtonAtex;

	buttonA.PushBack({ 0,0,15,15 });
	buttonA.speed = 0.F;
	buttonA.loop = false;
	currentAnimation = &buttonA;

}

ChestTrigger::~ChestTrigger()
{
	DeleteFromSubtiles(nSubtiles);
	myChest = nullptr;
}

void ChestTrigger::Draw()
{
	if (currentAnimation != nullptr && drawA)
	{
		App->render->Blit(entityTex, position.x - 5, position.y - 50, &currentAnimation->GetCurrentFrame(), 1.0F);
		drawA = false;
	}
	if (App->scene->debugSubtiles)
		DebugTrigger();
}


bool ChestTrigger::DoTriggerAction()
{
	drawA = true;

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
