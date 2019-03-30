#ifndef J1SCENEUI_H
#define J1SCENEUI_H
#include "j1Module.h"

class j1SceneUi :public j1Module 
{
public:
	j1SceneUi();
	~j1SceneUi();

	// Called before render is available
	bool Awake(pugi::xml_node& node);

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();
};
#endif
