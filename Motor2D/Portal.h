#ifndef _PORTAL_H_
#define _PORTAL_H_

#include "Trigger.h"
#include "j1Scene.h"
#include "Color.h"

class Portal : public Trigger
{
public:
	Portal(float posx, float posy, SceneState scene, Color color);
	~Portal();
	bool Update(float dt);
	void Draw();

	bool DoTriggerAction();
private:
	SceneState scene;
	Color color;

	Animation open;
	Animation idle;

	Animation*	currentAnim = nullptr;
	bool doit = false;

};


#endif