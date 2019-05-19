#include "DialogTrigger.h"
#include "j1DialogSystem.h"
#include "j1EntityFactory.h"
DialogTrigger::DialogTrigger(float posx, float posy, std::string Dtype):Trigger(TRIGGER_TYPE::DIALOG,posx,posy,"dialog"),dialogType(Dtype)
{
	nSubtiles = 1;
	SetPivot(0, 0);
	AssignInSubtiles(nSubtiles);
}

DialogTrigger::~DialogTrigger()
{
}

bool DialogTrigger::Update(float dt)
{
	if (PrevSubtile != App->entityFactory->player->GetSubtilePos())
		active = false;
	PrevSubtile = App->entityFactory->player->GetSubtilePos();
	return true;
}

void DialogTrigger::Draw()
{
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
	if (!active)
	{
		active = true;
		App->dialog->SetCurrentDialog(dialogType.data());
	}
	return true;
}
