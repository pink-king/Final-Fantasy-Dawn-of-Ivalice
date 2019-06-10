#include "Fade.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Window.h"
#include "p2Log.h"

Fade::Fade(float transition_time, bool is_scene_change, SceneState scene_to_change, Color color, bool debugTP, LvlPart lvlPart) : Transition(transition_time)
{
	this->is_scene_change = is_scene_change;
	this->scene_to_change = scene_to_change;
	this->color = color;
	this->debugTP = debugTP;
	this->lvlPart = lvlPart;

	uint width, height;

	App->win->GetWindowSize(width, height);
	screen = { 0, 0, (int)width, (int)height };
	SDL_SetRenderDrawBlendMode(App->render->renderer, SDL_BLENDMODE_BLEND);

	if(debugTP)
		App->SaveGame("save_door.xml");
}

Fade::~Fade()
{

}

void Fade::Entering()
{
	Transition::Entering();

	float normalized_alpha = floor(LerpValue(percent, 0, 255));
	DrawFadeRect(normalized_alpha);
}

void Fade::Action()
{
	DrawFadeRect(255.0F);

	if (!debugTP)
	{
		if (is_scene_change)
		{
			App->scene->LoadScene(scene_to_change);
			//DrawFadeRect(255.0F);
		}
	}
	else
		App->scene->DebugTP(scene_to_change,lvlPart);
	Transition::Action();
}

void Fade::Exiting()
{
	float normalized_alpha = floor(LerpValue(percent, 255, 0));

	if (normalized_alpha < 0)
		normalized_alpha = 255;

	DrawFadeRect(normalized_alpha);

	Transition::Exiting();
}

void Fade::DrawFadeRect(float alpha_value)
{
	if (alpha_value > 255)alpha_value = 255;
	if (alpha_value < 0)alpha_value = 0;

	SDL_SetRenderDrawColor(App->render->renderer, color.r, color.g, color.b, alpha_value);
	SDL_RenderFillRect(App->render->renderer, &screen);
}

void Fade::SetColor(Color color)
{
	this->color = color;
}
