#ifndef __j1MODULECAMERA2D_H__
#define __j1MODULECAMERA2D_H__

#include "j1Module.h"
#include "SDL/include/SDL_rect.h"



class j1ModuleCamera2D : public j1Module
{
public:
	j1ModuleCamera2D();
	~j1ModuleCamera2D();

	//core loops ------
	bool Awake(pugi::xml_node& node);
	bool Start();
	bool PreUpdate();
	bool Update(float dt);
	//bool PostUpdate();
	virtual bool CleanUp();

	// functionality ------


public:


private:
	SDL_Rect camera;
};

#endif //__J1MODULECAMERA2D_H__