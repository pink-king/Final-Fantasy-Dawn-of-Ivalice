#include "SaveTrigger.h"
#include "j1DialogSystem.h"
#include "j1Render.h"
#include "j1EntityFactory.h"
SaveTrigger::SaveTrigger(float posx, float posy) :Trigger(TRIGGER_TYPE::SAVE, posx, posy, "saveTrigger")
{
	entityTex = App->entityFactory->campFireTex;
	signTex = App->entityFactory->interactiveStatesTex;
	idle.PushBack({ 0,0,48,48 });
	idle.PushBack({ 48,0,48,48 });
	idle.PushBack({ 96,0,48,48 });
	idle.PushBack({ 144,0,48,48 });
	idle.PushBack({ 192,0,48,48 });
	idle.PushBack({ 0,48,48,48 });
	idle.PushBack({ 48,48,48,48 });
	idle.PushBack({ 96,48,48,48 });
	idle.speed = 10.F;

	sign.PushBack({ 0,96,32,32 });
	sign.PushBack({ 32,96,32,32 });
	sign.PushBack({ 64,96,32,32 });
	sign.PushBack({ 96,96,32,32 });
	sign.PushBack({ 0,128,32,32 });
	sign.PushBack({ 32,128,32,32 });
	sign.PushBack({ 64,128,32,32 });
	sign.PushBack({ 96,128,32,32 });
	sign.speed = 10.0F;

	saveSign.PushBack({128,0,32,32});
	saveSign.speed = 0.F;

	nSubtiles = 1;
	SetPivot(24, 42);
	size.create(48, 48);
	AssignInSubtiles(nSubtiles);
	currentAnim = &idle;

	currentAnimation = &sign;
}

SaveTrigger::~SaveTrigger()
{
	DeleteFromSubtiles(nSubtiles);
}

bool SaveTrigger::Update(float dt)
{
	if (isActive)
	{
		currentAnimation = &saveSign;
	}
	else
		currentAnimation = &sign;

	return true;
}

void SaveTrigger::Draw()
{
	if (currentAnim != nullptr)
		App->render->Blit(entityTex, position.x, position.y, &currentAnim->GetCurrentFrame(), 1.0F);

	if (currentAnimation != nullptr)
	{
		App->render->Blit(signTex, position.x + 8, position.y - 16, &currentAnimation->GetCurrentFrame(), 1.0F);
		if (isActive)
		{
			App->render->Blit(App->gui->GetAtlas(), position.x + 28, position.y - 18, &App->input->GetAssociatedRectForThisGamepadInput(
				App->input->gamepadScheme.sharedInput.interact.button,
				App->input->gamepadScheme.sharedInput.interact.axis), 1.0F, SDL_FLIP_NONE, 0.5F);
			isActive = false; 
		}
	}



	if (App->scene->debugSubtiles)
		DebugTrigger();
}

bool SaveTrigger::CleanUp()
{
	return true;
}

bool SaveTrigger::Save(pugi::xml_node &) const
{
	
	return true;
}

bool SaveTrigger::DoTriggerAction()
{
	isActive = true;
	if (App->input->GetControllerGeneralPress(App->input->gamepadScheme.sharedInput.interact) == KEY_DOWN)
	{
		isActive = true;
		App->dialog->SetCurrentDialog("SAVEGAME");
	}
	return true;
}
