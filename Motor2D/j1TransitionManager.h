#ifndef _TRANSITION_MANAGER_H_
#define _TRANSITION_MANAGER_H_

//This is the module that Marc will do on his research?

#include "j1Module.h"
#include "Color.h"
#include "Transition.h"
#include "j1Scene.h"

#include<list>

class j1TransitionManager :public j1Module
{
public:


private:
	std::list<Transition*> active_transitions;

	//Util variables for diverse transitions
	SDL_Rect screen;


public:
	j1TransitionManager();
	~j1TransitionManager();

	// Called before render is available
	virtual bool Awake(pugi::xml_node&);
	// Called before the first frame
	virtual bool Start();
	// Called each loop iteration
	virtual bool PostUpdate();
	// Called before quitting
	virtual bool CleanUp();

	//--------------------------------

	void CreateFadeTransition(float transition_time, bool is_scene_change, SceneState scene_to_transition, Color color = Black);
	void CreateSquaresTransition(float transition_time, bool is_scene_change, SceneState scene_to_transition, Color color = Black);
	void CreateDissolveTransition(float transition_time, SceneState scene_to_transition, LvlPart lvlPart = LvlPart::NO_PART);

	void DestroyTransition(Transition* transition_to_destroy);

	bool transitioning = false;
};

#endif
