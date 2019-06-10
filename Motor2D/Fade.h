#pragma once
#include "Transition.h"
#include "j1Scene.h"
class Fade :
	public Transition
{
private:
	Color color = Black;
	bool is_scene_change = false;
	bool debugTP = false;
	SceneState scene_to_change = SceneState::MAX_STATES;
	LvlPart lvlPart = LvlPart::NO_PART;
	SDL_Rect screen;
public:
	Fade(float transition_time, bool is_scene_change, SceneState scene_to_change, Color color = Black, bool debugTP = false, LvlPart lvlPart = LvlPart::NO_PART);
	~Fade();

	void Entering();
	void Action();
	void Exiting();

	void DrawFadeRect(float alpha_value);
	void SetColor(Color color);
};

