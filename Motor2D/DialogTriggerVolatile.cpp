#include "DialogTriggerVolatile.h"
#include "j1DialogSystem.h"

DialogTriggerVolatile::DialogTriggerVolatile(float posx, float posy, std::string dType, uint nSubtiles, DialogTriggerVolatile * parent) : dialogType(dType), parent(parent), Trigger(TRIGGER_TYPE::DIALOG_VOLATILE, posx, posy, "dialogVolatile")
{
	this->nSubtiles = nSubtiles; 
	AssignInSubtiles(nSubtiles);
}

DialogTriggerVolatile::~DialogTriggerVolatile()
{
	DeleteFromSubtiles(nSubtiles); 
}

bool DialogTriggerVolatile::PreUpdate()
{
	CheckParentDestroyed(); 
	return true;
}

bool DialogTriggerVolatile::DoTriggerAction()
{
	App->dialog->SetCurrentDialog(dialogType.data());
	to_delete = true; 
	if (parent != nullptr)
	{
		parent->to_delete = true;
	}

	return true;
}

void DialogTriggerVolatile::CheckParentDestroyed()
{
	if (parent != nullptr)
	{
		if (parent->to_delete == true)
			to_delete = true; 
	}
}
