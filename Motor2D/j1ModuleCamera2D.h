#ifndef __j1MODULECAMERA2D_H__
#define __j1MODULECAMERA2D_H__

#include "j1Module.h"
#include "SDL/include/SDL_rect.h"
#include "p2Point.h"
#include "j1Timer.h"



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
	void SetCameraPos(iPoint pos);
	void SetCameraPos(int x, int y);
	void SetCameraRect(SDL_Rect camRect);
	const SDL_Rect* GetCameraRectPtr() const;
private:
	fPoint lerp(fPoint v0, fPoint v1, float t);

	j1Timer lerpTimer;

public:
	SDL_Rect camera;
private:
};

#endif //__J1MODULECAMERA2D_H__