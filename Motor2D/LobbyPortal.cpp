#include "LobbyPortal.h"
#include "j1TransitionManager.h"
#include "j1EntityFactory.h"

LobbyPortal::LobbyPortal(float posx, float posy, SceneState scene, Color color)
	: Trigger(TRIGGER_TYPE::PORTAL, posx, posy, "portal"), scene(scene), color(color)
{
	nSubtiles = 2;
	SetPivot(0, 0);
	AssignInSubtiles(nSubtiles);
}

LobbyPortal::~LobbyPortal()
{
}

bool LobbyPortal::DoTriggerAction()
{
	App->scene->ComeToPortal = true;
	App->pause = true;
	App->transitionManager->CreateFadeTransition(1.0, true, scene, color);
	App->scene->previosState = App->scene->state;
	to_delete = true;
	return true;
}
