
#include "ExitPortal.h"
#include "j1TransitionManager.h"
#include "j1EntityFactory.h"

ExitPortal::ExitPortal(float posx, float posy)
	: Trigger(TRIGGER_TYPE::PORTAL, posx, posy, "Exitportal")
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
	App->pause = true;
	timer.Start();
	App->audio->PlayFx(App->scene->portal_vanish, 0);
}

ExitPortal::~ExitPortal()
{
	DeleteFromSubtiles(nSubtiles);
}

bool ExitPortal::Update(float dt)
{
	if (timer.Read() > 500)
	{
		App->pause = false;
	}
	if (idle.Finished() && currentAnim == &idle)
	{
		currentAnim = &close;
	}
	if (close.Finished())
	{
		to_delete = true;
	}
	return true;
}

void ExitPortal::Draw()
{
	if (currentAnim != nullptr)
		App->render->Blit(entityTex, position.x - size.x / 2, position.y - size.y, &currentAnim->GetCurrentFrame(), 1.0F);
	if (App->scene->debugSubtiles)
		DebugTrigger();
}

bool ExitPortal::DoTriggerAction()
{
	return true;
}