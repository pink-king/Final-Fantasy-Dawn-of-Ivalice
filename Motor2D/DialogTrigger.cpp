#include "DialogTrigger.h"
#include "j1DialogSystem.h"
#include "j1EntityFactory.h"
DialogTrigger::DialogTrigger(float posx, float posy, std::string Dtype):Trigger(TRIGGER_TYPE::DIALOG,posx,posy,"dialog"),dialogType(Dtype)
{
	nSubtiles = 0;
	SetPivot(0, 0);
	AssignInSubtiles(nSubtiles);
}

DialogTrigger::~DialogTrigger()
{
	DeleteFromSubtiles(nSubtiles); 
}

bool DialogTrigger::Update(float dt)
{
	
	return true;
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
	if(App->input->GetControllerButton(SDL_CONTROLLER_BUTTON_A) == KEY_DOWN || App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
		App->dialog->SetCurrentDialog(dialogType.data());

	return true;
}
