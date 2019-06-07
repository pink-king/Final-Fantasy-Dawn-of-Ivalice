#include "WinTrigger.h"
#include "j1TransitionManager.h"
#include "j1EntityFactory.h"

WinTrigger::WinTrigger(float posx, float posy, SceneState scene, Color color)
	: Trigger(TRIGGER_TYPE::PORTAL, posx, posy, "portal"), scene(scene), color(color)
{
	nSubtiles = 3;
	SetPivot(0, 0);
	AssignInSubtiles(nSubtiles);
}

WinTrigger::~WinTrigger()
{
	DeleteFromSubtiles(nSubtiles);
}

bool WinTrigger::DoTriggerAction()
{
	App->pause = true;
	App->transitionManager->CreateFadeTransition(1.0, true, scene, color);
	to_delete = true;
	App->scene->ComeToWin = true;
	return true;
}