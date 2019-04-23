#include "j1ModuleCamera2D.h"
#include "j1EntityFactory.h"
#include "j1Window.h"
#include "j1Render.h"
#include "Brofiler/Brofiler.h"

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
	gen.seed(rd()); //Standard mersenne_twister_engine seeded with rd()
	
	// TODO: vertical slice workaround
	camera.x = 2900;
	camera.y = -3250;
	preShakePos.x = camera.x;
	preShakePos.y = camera.y;
	//lerpTimer.Start();
	return true;
}

bool j1ModuleCamera2D::PreUpdate()
{
	if (App->input->GetKey(SDL_SCANCODE_F7) == KEY_DOWN)
		debug = !debug; 
	return true;
}

bool j1ModuleCamera2D::PostUpdate()
{
	BROFILER_CATEGORY("Camera Update", Profiler::Color::Gainsboro);

	LOG("camera: %i, %i:", camera.x, camera.y);

	if (App->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN)
	{
		AddTrauma(0.13f);
	}

	// test - figuring out camera core functionality
	// TODO: 
	// Camera shakes get "noise" from perlin noise instead of floating random values
	// camera transitions
	// clamping to desired entity target (framing and point of interest)
	// zooming
	if (trauma > 0) // reposition camera to original poss after shake, while we have trauma
	{
		camera.x = preShakePos.x;
		camera.y = preShakePos.y;
	}
	// follow player
	float smooth = 3.0f;
	fPoint offset{ 640,360 }; // pivot of the "screen" itself at center pos
	offset.y += 30.0f; // offset for character height compensation

	fPoint playerPos;
	if (App->entityFactory->player != nullptr)
	{
		playerPos = App->entityFactory->player->GetPivotPos() * App->win->GetScale();
		fPoint test;
		test.x = camera.x;
		test.y = camera.y;

		test = lerp(test, { -playerPos.x + offset.x, -playerPos.y + offset.y }, 0.016f * smooth);
		camera.x = test.x;
		camera.y = test.y;
	}

	// shake
	// if we have current trauma
	if (trauma > 0)
	{
		preShakePos.x = camera.x;
		preShakePos.y = camera.y;

		camera.x += maxOffset * GetShakeAmount() * GetFloatNegOneToOne();
		camera.y += maxOffset * GetShakeAmount() * GetFloatNegOneToOne();
		
		if (debug)
		{
			/*LOG("Trauma:%f", trauma);
			LOG("Shake amount: %f", GetShakeAmount());*/

			uint w, h;
			App->win->GetWindowSize(w, h);
			SDL_Rect traumaRect = { 10,h - 10, 20, -(trauma * (h - 20)) };
			SDL_Rect shakeRect = { 30, h - 10, 20, -(GetShakeAmount() * (h - 20)) };

			SDL_SetRenderDrawColor(App->render->renderer, 255, 132, 64, 200);
			SDL_RenderFillRect(App->render->renderer, &traumaRect);
			SDL_SetRenderDrawColor(App->render->renderer, 107, 186, 255, 200);
			SDL_RenderFillRect(App->render->renderer, &shakeRect);
		}

		// decay trauma
		trauma -= App->GetDt() * traumaDecay * (trauma + 0.3f);
	}
	else // update preshake pos
	{
		preShakePos.x = camera.x;
		preShakePos.y = camera.y;
	}

	return true;
}

bool j1ModuleCamera2D::Update(float dt)
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

iPoint j1ModuleCamera2D::GetCamPos() const
{
	return { camera.x, camera.y };
}

float j1ModuleCamera2D::GetFloatNegOneToOne()
{
	std::uniform_real_distribution<float> dis(-1.0f, std::nextafter(1.0f, DBL_MAX));

	return dis(gen);
}

float j1ModuleCamera2D::AddTrauma(float value)
{
	// values must be between 0 and 1 (percentages)
	if (value <= 1.0f)
	{
		trauma += value;
		return trauma = MAX(0.0f, MIN(trauma, 1.0f));
	}
	
	return 0.0f;
}

float j1ModuleCamera2D::GetShakeAmount() const
{
	return trauma * trauma;
}