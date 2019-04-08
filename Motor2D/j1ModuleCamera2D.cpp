#include "j1ModuleCamera2D.h"
#include "j1EntityFactory.h"
#include "j1Window.h"
#include "j1Render.h"

j1ModuleCamera2D::j1ModuleCamera2D()
{
	name.assign("ModuleCamera2D");
}

j1ModuleCamera2D::~j1ModuleCamera2D()
{
}

bool j1ModuleCamera2D::Awake(pugi::xml_node & node)
{
	return true;
}

bool j1ModuleCamera2D::Start()
{
	lerpTimer.Start();
	return true;
}

bool j1ModuleCamera2D::PreUpdate()
{
	return true;
}

bool j1ModuleCamera2D::Update(float dt)
{
	// test - figuring out camera core functionality
	// todo: 
	// camera transitions (different types of shakes)
	// clamping to desired entity target
	// zooming
	float smooth = 3.0f;
	fPoint offset{ 640,360 }; // pivot of the "screen" itself at center pos
	offset.y += 30.0f; // offset for character height compensation

	fPoint playerPos = App->entityFactory->player->GetPivotPos() * App->win->GetScale();

	fPoint test;
	test.x = camera.x;
	test.y = camera.y;

	test = lerp(test, { -playerPos.x + offset.x, -playerPos.y + offset.y }, 0.016f * smooth);
	camera.x = test.x;
	camera.y = test.y;

	return true;
}

bool j1ModuleCamera2D::PostUpdate()
{
	return true;
}

bool j1ModuleCamera2D::CleanUp()
{
	return true;
}

// Utils ----------------------------------------------------------

void j1ModuleCamera2D::SetCameraPos(iPoint pos)
{
	camera.x = pos.x;
	camera.y = pos.y;
}

void j1ModuleCamera2D::SetCameraPos(int x, int y)
{
	camera.x = x;
	camera.y = y;
}

void j1ModuleCamera2D::SetCameraRect(SDL_Rect camRect)
{
	camera = camRect;
}

const SDL_Rect* j1ModuleCamera2D::GetCameraRectPtr() const
{
	return &camera;
}

fPoint j1ModuleCamera2D::lerp(fPoint v0, fPoint v1, float t)
{
	return v0*(1 - t) + v1*t;
}
