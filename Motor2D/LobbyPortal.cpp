#include "LobbyPortal.h"
#include "j1TransitionManager.h"
#include "j1EntityFactory.h"

LobbyPortal::LobbyPortal(float posx, float posy, SceneState scene, Color color)
	: Trigger(TRIGGER_TYPE::PORTAL, posx, posy, "portal"), scene(scene), color(color)
{
	entityTex = App->entityFactory->portalTex;

	idle.PushBack({ 0,0,64,64 });
	idle.PushBack({ 64,0,64,64 });
	idle.PushBack({ 128,0,64,64 });
	idle.PushBack({ 192,0,64,64 });
	idle.PushBack({ 256,0,64,64 });
	idle.PushBack({ 320,0,64,64 });
	idle.PushBack({ 384,0,64,64 });
	idle.PushBack({ 448,0,64,64 });
	idle.speed = 10.F;
	idle.loop = true;

	close.PushBack({ 0,128,64,64 });
	close.PushBack({ 64,128,64,64 });
	close.PushBack({ 128,128,64,64 });
	close.PushBack({ 192,128,64,64 });
	close.PushBack({ 256,128,64,64 });
	close.PushBack({ 320,128,64,64 });
	close.speed = 10.F;
	close.loop = false;

	nSubtiles = 1;
	SetPivot(0, 0);
	size.create(64, 64);

	AssignInSubtiles(nSubtiles);

	currentAnim = &idle;
	App->audio->PlayFx(App->entityFactory->portal_appear, 0);
	App->audio->PlayFx(App->entityFactory->portal_mantain, -1);

}

LobbyPortal::~LobbyPortal()
{
	DeleteFromSubtiles(nSubtiles);
}

bool LobbyPortal::Update(float dt)
{
	if (currentAnim == &idle)
		doit = true;
	return true;
}

void LobbyPortal::Draw()
{
	if (currentAnim != nullptr)
		App->render->Blit(entityTex, position.x - size.x / 2, position.y - size.y, &currentAnim->GetCurrentFrame(), 1.0F);
}

bool LobbyPortal::DoTriggerAction()
{
	if (doit)
	{
		App->audio->PlayFx(App->entityFactory->portal_travel, 0);
		currentAnim = &close;
		App->scene->ComeToPortal = true;
		App->transitionManager->CreateFadeTransition(1.0, true, scene, color);
		App->scene->previosState = App->scene->state;
		doit = false;
	}
	return true;
}
