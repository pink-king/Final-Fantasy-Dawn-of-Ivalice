#include "Portal.h"
#include "j1TransitionManager.h"
#include "j1EntityFactory.h"
#include "j1Render.h"

Portal::Portal(float posx, float posy, SceneState scene, Color color)
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

	open.PushBack({ 0,64,64,64 });
	open.PushBack({ 64,64,64,64 });
	open.PushBack({ 128,64,64,64 });
	open.PushBack({ 192,64,64,64 });
	open.PushBack({ 256,64,64,64 });
	open.PushBack({ 320,64,64,64 });
	open.PushBack({ 384,64,64,64 });
	open.PushBack({ 448,64,64,64 });
	open.speed = 10.F;
	open.loop = false;

	nSubtiles = 1;
	SetPivot(0, 0);
	size.create(64, 64);
	AssignInSubtiles(nSubtiles);
	currentAnim = &open;
	App->audio->PlayFx(App->scene->portal_appear, 0);
	App->audio->PlayFx(App->scene->portal_mantain, -1);
}

Portal::~Portal()
{
	DeleteFromSubtiles(nSubtiles);
}

bool Portal::Update(float dt)
{

	if (open.Finished() && currentAnim != &idle)
	{
		doit = true;
		currentAnim = &idle;
	}
	return true;
}

void Portal::Draw()
{
	if (currentAnim != nullptr)
		App->render->Blit(entityTex, position.x - size.x / 2, position.y - size.y, &currentAnim->GetCurrentFrame(), 1.0F);
	if (App->scene->debugSubtiles)
		DebugTrigger();
}


bool Portal::DoTriggerAction()
{
	App->scene->portalPos = { position.x,position.y + 16 };
	if (doit)
	{
		App->entityFactory->player->to_delete = true;
		App->SaveGame("Portal.xml");
		App->scene->ComeToPortal = true;
		App->pause = true;
		App->transitionManager->CreateFadeTransition(1.0, true, scene, color);
		App->scene->previosState = App->scene->state;
		doit = false;
	}
	return true;
}
