#include "Dissolve.h"
#include "j1Render.h"
#include "j1Window.h"
#include "p2Log.h"
#include "j1Render.h"

#include "j1App.h"

Dissolve::Dissolve(float transition_time, SceneState scene_to_change) : Transition(transition_time)
{
	this->scene_to_change = scene_to_change;
	this->color = color;

	uint width, height;
	int w, h;
	App->win->GetWindowSize(width, height);

	rect = { 0, 0, (int)width, (int)(height) };
	SDL_SetRenderDrawBlendMode(App->render->renderer, SDL_BLENDMODE_BLEND);

	target_text = SDL_CreateTexture(App->render->renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, width, height);
	SDL_SetTextureBlendMode(target_text, SDL_BLENDMODE_BLEND);

	capture_surf = SDL_CreateRGBSurface(0, width, height-19, 32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);
	SDL_RenderReadPixels(App->render->renderer, &rect, SDL_GetWindowPixelFormat(App->win->window), capture_surf->pixels, capture_surf->pitch);
	SDL_SaveBMP(capture_surf, "screenshot.bmp");
	capture_text = App->tex->Load("screenshot.bmp");
	if (capture_text == NULL) {
		fprintf(stderr, "CreateTextureFromSurface failed: %s\n", SDL_GetError());
		exit(1);
	}
	// Destroy the screenshot surface
	SDL_FreeSurface(capture_surf);

}

Dissolve::~Dissolve()
{
}

void Dissolve::Entering()
{
	state = TransitionState::ACTION;
}

void Dissolve::Action()
{
	Transition::Action();

	App->scene->LoadScene(scene_to_change);
}

void Dissolve::Exiting()
{

	float alpha = LerpValue(percent, 255, 0);
	int amod = SDL_SetTextureAlphaMod(capture_text, alpha);
	LOG("amod %i alpha %f", amod, alpha);

	//Show rendered to texture
	SDL_RenderCopy(App->render->renderer, capture_text, NULL, NULL);

	Transition::Exiting();
}
