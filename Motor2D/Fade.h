#pragma once
#include "Transition.h"
#include "j1Scene.h"
class Fade :
	public Transition
{
private:
	Color color = Black;
	bool is_scene_change = false;
	SceneState scene_to_change = SceneState::MAX_STATES;

	SDL_Rect screen;
public:
	Fade(float transition_time, bool is_scene_change, SceneState scene_to_change, Color color = Black);
	~Fade();

	void Entering();
	void Action();
	void Exiting();

	void DrawFadeRect(float alpha_value);
	void SetColor(Color color);
};

