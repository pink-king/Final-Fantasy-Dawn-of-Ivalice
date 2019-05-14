#pragma once
#include "Transition.h"
#include "j1Textures.h"
#include "j1Scene.h"

class Dissolve :
	public Transition
{
private:
	Color color = Black;
	bool is_scene_change = false;
	SceneState scene_to_change = SceneState::MAX_STATES;

	uint width, height;

	SDL_Texture* target_text = nullptr;
	SDL_Texture* capture_text = nullptr;
	SDL_Surface* capture_surf = nullptr;
	SDL_Rect rect;

public:
	Dissolve(float transition_time, SceneState scene_to_change);
	~Dissolve();

	void Entering();
	void Action();
	void Exiting();

};
