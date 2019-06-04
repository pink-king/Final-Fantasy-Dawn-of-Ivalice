#include "Door.h"
#include "j1TransitionManager.h"
#include "j1EntityFactory.h"

Door::Door(float posx, float posy, SceneState scene, Color color, uint nSubtiles,bool lookLeft)
	: Trigger(TRIGGER_TYPE::PORTAL, posx, posy, "portal"), scene(scene), color(color)
{
	this->nSubtiles = nSubtiles;
	SetPivot(0,-10);
	AssignInSubtiles(nSubtiles);

	entityTex = App->entityFactory->hallTex;
	
	openL.PushBack({ 224,64,32,49 });
	openL.speed = 0.F;
	openL.loop = false;

	openR.PushBack({ 128,0,32,49 });
	openR.speed = 0.F;
	openR.loop = false;

	if (lookLeft)
		currentAnim = &openL;
	else
		currentAnim = &openR;
}

Door::~Door()
{

}

void Door::Draw()
{
	if (currentAnim != nullptr)
		App->render->Blit(entityTex, position.x - 16, position.y - 40, &currentAnim->GetCurrentFrame(), 1.0F);

	if (active)
	{
		App->render->Blit(App->gui->GetAtlas(), position.x - 8, position.y - 59, &App->input->GetAssociatedRectForThisGamepadInput(
			App->input->gamepadScheme.sharedInput.interact.button,
			App->input->gamepadScheme.sharedInput.interact.axis), 1.0F, SDL_FLIP_NONE, 0.5F);
	/*	App->entityFactory->player->position.x + 15,
			App->entityFactory->player->position.y - 10,*/
		active = false;
	}

	if (App->scene->debugSubtiles)
		DebugTrigger();
}

bool Door::DoTriggerAction()
{
	active = true;
	if (App->input->GetControllerGeneralPress(App->input->gamepadScheme.sharedInput.interact) == KEY_DOWN || App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	{
		App->SaveGame("save_game.xml");
		App->scene->ComeToDeath = true;
		App->scene->ComeToWin = true;
		App->pause = true;
		App->transitionManager->CreateFadeTransition(2.0, true, scene, color);
		App->scene->previosState = App->scene->state;
	}
	return true;
}