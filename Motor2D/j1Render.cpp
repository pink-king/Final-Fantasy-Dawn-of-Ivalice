#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Window.h"
#include "j1Render.h"
#include "j1ModuleCamera2D.h"
#include "j1Map.h"

j1Render::j1Render() : j1Module()
{
	name.assign("renderer");
	background.r = 0;
	background.g = 0;
	background.b = 0;
	background.a = 0;
}

// Destructor
j1Render::~j1Render()
{}

// Called before render is available
bool j1Render::Awake(pugi::xml_node& config)
{
	LOG("Create SDL rendering context");
	bool ret = true;
	// load flags
	Uint32 flags = SDL_RENDERER_ACCELERATED;

	if(config.child("vsync").attribute("value").as_bool(true) == false)
	{
		flags |= SDL_RENDERER_PRESENTVSYNC;
		LOG("Using vsync");
		
	}

	renderer = SDL_CreateRenderer(App->win->window, -1, flags);
	uint w, h;
	App->win->GetWindowSize(w, h);
	SDL_RenderSetLogicalSize(renderer, w, h);

	if(renderer == NULL)
	{
		LOG("Could not create the renderer! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	else
	{
		camera = App->camera2D->GetCameraRectPtr();
		App->camera2D->SetCameraRect({ 0,0, (int)w, (int)h});
		/*camera.w = App->win->screen_surface->w;
		camera.h = App->win->screen_surface->h;
		camera.x = 0;
		camera.y = 0;*/
	}

	return ret;
}

// Called before the first frame
bool j1Render::Start()
{
	LOG("render start");
	// back background
	SDL_RenderGetViewport(renderer, &viewport);
	return true;
}

// Called each loop iteration
bool j1Render::PreUpdate()
{
	SDL_RenderClear(renderer);
	return true;
}

bool j1Render::Update(float dt)
{
	return true;
}

bool j1Render::PostUpdate()
{
	SDL_SetRenderDrawColor(renderer, background.r, background.g, background.b, background.a);
	SDL_RenderPresent(renderer);
	return true;
}

// Called before quitting
bool j1Render::CleanUp()
{
	LOG("Destroying SDL render");
	SDL_DestroyRenderer(renderer);
	return true;
}

// Load Game State
bool j1Render::Load(pugi::xml_node& data)
{
	/*camera.x = data.child("camera").attribute("x").as_int();
	camera.y = data.child("camera").attribute("y").as_int();*/

	return true;
}

// Save Game State
bool j1Render::Save(pugi::xml_node& data) const
{
	pugi::xml_node cam = data.append_child("camera");

	/*cam.append_attribute("x") = camera.x;
	cam.append_attribute("y") = camera.y;*/

	return true;
}

void j1Render::SetBackgroundColor(SDL_Color color)
{
	background = color;
}

void j1Render::SetTextureColor(SDL_Texture* tex, uint r, uint g, uint b)
{
	SDL_SetTextureColorMod(tex, r, g, b);
}

void j1Render::SetTextureAlpha(SDL_Texture* tex, uint alpha)
{
	SDL_SetTextureAlphaMod(tex, alpha);
}

void j1Render::SetViewPort(const SDL_Rect& rect)
{
	SDL_RenderSetViewport(renderer, &rect);
}

void j1Render::ResetViewPort()
{
	SDL_RenderSetViewport(renderer, &viewport);
}

iPoint j1Render::ScreenToWorld(int x, int y) const
{
	iPoint ret;
	int scale = App->win->GetScale();
	
	ret.x = (x - camera->x / scale);
	ret.y = (y - camera->y / scale);

	return ret;
}

iPoint j1Render::WorldToScreen(int x, int y, bool descriptions) const
{

	if (!descriptions)
		return iPoint(x * App->win->GetScale() + camera->x, y * App->win->GetScale() + camera->y);
	else
		return iPoint(x * App->win->GetScale(), y * App->win->GetScale());
}

bool j1Render::IsOnCamera(const int & x, const int & y, const int & w, const int & h) const
{
	int scale = App->win->GetScale();

	SDL_Rect r = { x*scale,y*scale,w*scale,h*scale };
	SDL_Rect cam = { -camera->x,-camera->y,camera->w,camera->h};

	return SDL_HasIntersection(&r, &cam);
}
/*
// Blit to screen
bool j1Render::Blit(SDL_Texture* texture, int x, int y, const SDL_Rect* section, float speed, SDL_RendererFlip flip, float spriteScale,double angle, int pivot_x, int pivot_y) const
{
	bool ret = true;
	uint scale = App->win->GetScale();
	SDL_Rect rect;
	rect.x = (int)(camera->x * speed) + x * scale;
	rect.y = (int)(camera->y * speed) + y * scale;

	if(section != NULL)
	{
		rect.w = section->w * spriteScale;
		rect.h = section->h * spriteScale;
	}
	else
	{
		SDL_QueryTexture(texture, NULL, NULL, &rect.w, &rect.h);
	}

	rect.w *= scale;
	rect.h *= scale;

	SDL_Point* p = NULL;
	SDL_Point pivot;

	if(pivot_x != INT_MAX && pivot_y != INT_MAX)
	{
		pivot.x = pivot_x;
		pivot.y = pivot_y;
		p = &pivot;
	}

	if(SDL_RenderCopyEx(renderer, texture, section, &rect, angle, p, flip) != 0)
	{
		LOG("Cannot blit to screen. SDL_RenderCopy error: %s", SDL_GetError());
		ret = false;
	}

	return ret;
}*/

bool j1Render::Blit(SDL_Texture* texture, int x, int y, const SDL_Rect* section, float speed, SDL_RendererFlip flip, float spriteScale, double angle, int pivot_x, int pivot_y, bool useWindowScale) const
{
	bool ret = true;
	uint scale = App->win->GetScale();
	SDL_Rect rect;
	if (useWindowScale)
	{
		rect.x = (int)(camera->x * speed) + x * scale;
		rect.y = (int)(camera->y * speed) + y * scale;
	}
	else
	{
		rect.x = (int)(camera->x * speed) + x;
		rect.y = (int)(camera->y * speed) + y;
	}


	if (section != NULL)
	{
		rect.w = section->w * spriteScale;
		rect.h = section->h * spriteScale;
	}
	else
	{
		SDL_QueryTexture(texture, NULL, NULL, &rect.w, &rect.h);
	}

	rect.w *= scale;
	rect.h *= scale;

	SDL_Point* p = NULL;
	SDL_Point pivot;

	if (pivot_x != INT_MAX && pivot_y != INT_MAX)
	{
		pivot.x = pivot_x;
		pivot.y = pivot_y;
		p = &pivot;
	}

	if (SDL_RenderCopyEx(renderer, texture, section, &rect, angle, p, flip) != 0)
	{
		LOG("Cannot blit to screen. SDL_RenderCopy error: %s", SDL_GetError());
		ret = false;
	}

	return ret;
}
bool j1Render::BlitGui(SDL_Texture * texture, int x, int y, const SDL_Rect * section, float speed, float scaleFactor, float flippingAngle, SDL_Rect wantedRect) const
{
	bool ret = true;

	SDL_Rect rect;
	rect.x = (int)(camera->x * speed) + x;
	rect.y = (int)(camera->y * speed) + y;

	if (section != NULL)
	{
		rect.w = section->w;
		rect.h = section->h;

	}

	else
	{
		SDL_QueryTexture(texture, NULL, NULL, &rect.w, &rect.h);
	}


	if (wantedRect.h && wantedRect.w)       // adjust texture rect position and dimensions
	{
		rect.h = wantedRect.h;
		rect.w = wantedRect.w;
		rect.x = wantedRect.x;
		rect.y = wantedRect.y;

	}




	rect.w *= scaleFactor;
	rect.h *= scaleFactor;           // a resized image rect does not have the same size as the section.



	if (SDL_RenderCopyEx(renderer, texture, section, &rect, flippingAngle, 0, SDL_FLIP_NONE) != 0)
	{
		LOG("Cannot blit to screen. SDL_RenderCopy error: %s", SDL_GetError());
		ret = false;
	}


	return ret;
}

bool j1Render::DrawQuad(const SDL_Rect& rect, Uint8 r, Uint8 g, Uint8 b, Uint8 a, bool filled, bool use_camera) const
{
	bool ret = true;
	uint scale = App->win->GetScale();

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(renderer, r, g, b, a);

	SDL_Rect rec(rect);
	if(use_camera)
	{
		rec.x = (int)(camera->x + rect.x * scale);
		rec.y = (int)(camera->y + rect.y * scale);
		rec.w *= scale;
		rec.h *= scale;
	}

	int result = (filled) ? SDL_RenderFillRect(renderer, &rec) : SDL_RenderDrawRect(renderer, &rec);

	if(result != 0)
	{
		LOG("Cannot draw quad to screen. SDL_RenderFillRect error: %s", SDL_GetError());
		ret = false;
	}

	return ret;
}

bool j1Render::DrawIsoQuad(SDL_Rect rect, SDL_Color color) const
{
	bool ret = true;

	//Make the quad isometric
	iPoint p1 = App->map->WorldToIso(0, 0);
	iPoint p2 = App->map->WorldToIso(0 + rect.w, 0);
	iPoint p3 = App->map->WorldToIso(0 + rect.w, 0 + rect.h);
	iPoint p4 = App->map->WorldToIso(0, 0 + rect.h);

	//Place the quad on the correct position
	p1.x += rect.x;
	p1.y += rect.y;
	p2.x += rect.x;
	p2.y += rect.y;
	p3.x += rect.x;
	p3.y += rect.y;
	p4.x += rect.x;
	p4.y += rect.y;
	
	//Draw the quad
 	App->render->DrawLine(p1.x, p1.y, p2.x, p2.y, color.r, color.g, color.b, color.a, true);
	App->render->DrawLine(p2.x, p2.y, p3.x, p3.y, color.r, color.g, color.b, color.a, true);
	App->render->DrawLine(p3.x, p3.y, p4.x, p4.y, color.r, color.g, color.b, color.a, true);
 	App->render->DrawLine(p4.x, p4.y, p1.x, p1.y, color.r, color.g, color.b, color.a, true);
	

	return ret;
}

bool j1Render::DrawLine(int x1, int y1, int x2, int y2, Uint8 r, Uint8 g, Uint8 b, Uint8 a, bool use_camera) const
{
	bool ret = true;
	uint scale = App->win->GetScale();

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(renderer, r, g, b, a);

	int result = -1;

	if(use_camera)
		result = SDL_RenderDrawLine(renderer, camera->x + x1 * scale, camera->y + y1 * scale, camera->x + x2 * scale, camera->y + y2 * scale);
	else
		result = SDL_RenderDrawLine(renderer, x1 * scale, y1 * scale, x2 * scale, y2 * scale);

	if(result != 0)
	{
		LOG("Cannot draw quad to screen. SDL_RenderFillRect error: %s", SDL_GetError());
		ret = false;
	}

	return ret;
}

bool j1Render::DrawCircle(int x, int y, int radius, Uint8 r, Uint8 g, Uint8 b, Uint8 a, bool use_camera) const
{
	bool ret = true;
	uint scale = App->win->GetScale();

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(renderer, r, g, b, a);

	int result = -1;
	SDL_Point points[360];

	float factor = (float)M_PI / 180.0f;

	for(uint i = 0; i < 360; ++i)
	{
		if (use_camera)
		{
			points[i].x = (int)(x + radius * cos(i * factor));
			points[i].y = (int)(y + radius * sin(i * factor));
		}
		else
		{
			points[i].x = (int)((x + camera->x) + radius * cos(i * factor));
			points[i].y = (int)((y + camera->y) + radius * sin(i * factor));
		}
	}

	result = SDL_RenderDrawPoints(renderer, points, 360);

	if(result != 0)
	{
		LOG("Cannot draw quad to screen. SDL_RenderFillRect error: %s", SDL_GetError());
		ret = false;
	}

	return ret;
}