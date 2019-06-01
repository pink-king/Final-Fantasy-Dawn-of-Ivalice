#include "WinTrigger.h"
#include "j1TransitionManager.h"
#include "j1EntityFactory.h"

WinTrigger::WinTrigger(float posx, float posy, SceneState scene, Color color, uint nSubtiles)
	: Trigger(TRIGGER_TYPE::PORTAL, posx, posy, "portal"), scene(scene), color(color)
{
	this->nSubtiles = nSubtiles;
	SetPivot(0, 0);
	AssignInSubtiles(nSubtiles);
}

WinTrigger::~WinTrigger()
{
	DeleteFromSubtiles(nSubtiles);
}

bool WinTrigger::DoTriggerAction()
{
	App->SaveGame("save_game.xml");
	App->scene->ComeToDeath = true;
	App->scene->ComeToWin = true;
	App->pause = true;
	App->transitionManager->CreateFadeTransition(2.0, true, scene, color);
	App->scene->previosState = App->scene->state;
	to_delete = true;
	return true;
}