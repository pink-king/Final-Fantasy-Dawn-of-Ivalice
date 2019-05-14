#include "j1TransitionManager.h"
#include "Fade.h"
#include "Squares.h"
#include "Dissolve.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Window.h"
#include "p2Log.h"

j1TransitionManager::j1TransitionManager()
{
}


j1TransitionManager::~j1TransitionManager()
{
	App->pause = false;
}

bool j1TransitionManager::Awake(pugi::xml_node &)
{
	LOG("Awakening Transition Manager");
	return true;
}

bool j1TransitionManager::Start()
{
	LOG("Starting Transition Manager");

	return true;
}

bool j1TransitionManager::PostUpdate()
{
	for each(Transition* t in active_transitions)
	{
		t->PostUpdate();
	}

	return true;
}

bool j1TransitionManager::CleanUp()
{
	for (std::list<Transition*>::iterator iter = active_transitions.begin(); iter != active_transitions.end(); ++iter)
	{
		delete *iter;
		*iter = nullptr;
	}
	active_transitions.clear();

	return true;
}

void j1TransitionManager::CreateFadeTransition(float transition_time, bool is_scene_change, SceneState scene_to_transition, Color color)
{
	active_transitions.push_back(new Fade(transition_time, is_scene_change, scene_to_transition, color));
}

void j1TransitionManager::CreateSquaresTransition(float transition_time, bool is_scene_change, SceneState scene_to_transition, Color color)
{
	active_transitions.push_back(new Squares(transition_time, is_scene_change, scene_to_transition, color));
}

void j1TransitionManager::CreateDissolveTransition(float transition_time, SceneState scene_to_transition)
{
	active_transitions.push_back(new Dissolve(transition_time, scene_to_transition));
}



void j1TransitionManager::DestroyTransition(Transition * transition_to_destroy)
{
	active_transitions.remove(transition_to_destroy);
	delete transition_to_destroy;
	transition_to_destroy = nullptr;
}