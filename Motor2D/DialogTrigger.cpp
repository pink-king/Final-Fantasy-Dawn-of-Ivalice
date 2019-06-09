#include "DialogTrigger.h"
#include "j1DialogSystem.h"
#include "j1EntityFactory.h"
DialogTrigger::DialogTrigger(float posx, float posy, std::string Dtype, iPoint posState, uint nSubtiles, bool pressA):Trigger(TRIGGER_TYPE::DIALOG,posx,posy,"dialog"),dialogType(Dtype), pressA(pressA),posState(posState)
{
	this->nSubtiles = nSubtiles;
	SetPivot(0, 0);
	AssignInSubtiles(nSubtiles);
	entityTex = App->entityFactory->interactiveStatesTex;

	dialogSign.PushBack({ 0,96,32,32 });
	dialogSign.PushBack({ 32,96,32,32 });
	dialogSign.PushBack({ 64,96,32,32 });
	dialogSign.PushBack({ 96,96,32,32 });
	dialogSign.PushBack({ 0,128,32,32 });
	dialogSign.PushBack({ 32,128,32,32 });
	dialogSign.PushBack({ 64,128,32,32 });
	dialogSign.PushBack({ 96,128,32,32 });

	dialogSign.speed = 10.0F;
	//dialogSign.loop = true;

	onTrigger.PushBack({96,64,32,32});
	
	if(pressA)
		currentAnim = &dialogSign;
}

DialogTrigger::~DialogTrigger()
{
	DeleteFromSubtiles(nSubtiles); 
}

bool DialogTrigger::Update(float dt)
{
	if (active)
	{
		currentAnim = &onTrigger;
		active = false;
	}
	else
		currentAnim = &dialogSign;
	return true;
}

void DialogTrigger::Draw()
{
	if (currentAnim != nullptr)
	{
		App->render->Blit(entityTex, posState.x - 16, posState.y - 32, &currentAnim->GetCurrentFrame(), 1.0F);
		if (currentAnim == &onTrigger)
		{
			App->render->Blit(App->gui->GetAtlas(), posState.x + 4, posState.y - 34, &App->input->GetAssociatedRectForThisGamepadInput(
				App->input->gamepadScheme.sharedInput.interact.button,
				App->input->gamepadScheme.sharedInput.interact.axis), 1.0F, SDL_FLIP_NONE, 0.5F);
		}
	}

	if (App->scene->debugSubtiles)
		DebugTrigger();
}



bool DialogTrigger::CleanUp()
{
	return true;
}

bool DialogTrigger::Save(pugi::xml_node &) const
{
	return true;
}

bool DialogTrigger::DoTriggerAction()
{
	
	if (pressA && currentAnim != nullptr)
	{
		active = true;
	}
	if(((App->input->GetControllerGeneralPress(App->input->gamepadScheme.sharedInput.interact) == KEY_DOWN || App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN) && pressA && !App->scene->pausePanel->enable && !App->scene->controlsPanel->enable && !App->scene->inventory->enable && !App->scene->settingPanel->enable) || !pressA)
		App->dialog->SetCurrentDialog(dialogType.data());
	if (!pressA)
		to_delete = true;
	return true;
}
