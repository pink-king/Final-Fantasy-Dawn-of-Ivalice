#include "Portal.h"
#include "j1TransitionManager.h"
#include "j1EntityFactory.h"

Portal::Portal(float posx, float posy, SceneState scene, Color color)
	: Trigger(TRIGGER_TYPE::PORTAL, posx, posy, "portal"), scene(scene), color(color)
{
	nSubtiles = 2;
	SetPivot(0, 0);
	AssignInSubtiles(nSubtiles);
}

Portal::~Portal()
{
}

bool Portal::DoTriggerAction()
{
	App->SaveGame("Portal.xml");
	App->scene->ComeToPortal = true;
	App->pause = true;
	App->transitionManager->CreateFadeTransition(1.0, true,scene,color);
	App->scene->previosState = App->scene->state;
	App->scene->portalPos = position;
	to_delete = true;
	return true;
}
