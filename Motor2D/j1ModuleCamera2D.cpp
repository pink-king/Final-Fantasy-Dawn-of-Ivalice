#include "j1ModuleCamera2D.h"
#include "j1EntityFactory.h"
#include "j1Window.h"

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
	// test - figuring out
	float smooth = 1.0f;

	fPoint playerPos = App->entityFactory->player->GetPivotPos();

	fPoint test;
	test.x = camera.x;
	test.y = camera.y;

	test = lerp(test, { -playerPos.x, -playerPos.y }, dt * smooth);
	camera.x = test.x;
	camera.y = test.y;

	LOG("Camera: %i,%i", camera.x, camera.y);
	LOG("Player: %f,%f", playerPos.x, playerPos.y);

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
