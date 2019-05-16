#include "Portal.h"
#include "j1TransitionManager.h"
#include "j1EntityFactory.h"

Portal::Portal(float posx, float posy) : Trigger(TRIGGER_TYPE::PORTAL, posx, posy, "portal")
{
	nSubtiles = 2;
	SetPivot(0, 0);
	AssignInSubtiles(nSubtiles);
}

Portal::~Portal()
{
	App->entityFactory->CreateTrigger(TRIGGER_TYPE::PORTAL, position.x, position.y);
}

bool Portal::DoTriggerAction()
{
	App->pause = true;
	App->transitionManager->CreateFadeTransition(1.0, true,SceneState::LEVEL1,White);
	to_delete = true;
	return true;
}
