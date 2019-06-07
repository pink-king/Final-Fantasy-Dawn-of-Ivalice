#ifndef _DOOR_H_
#define _DOOR_H_

#include "Trigger.h"
#include "j1Scene.h"
#include "Color.h"

class Door : public Trigger
{
public:
	Door(float posx, float posy, SceneState scene, Color color, uint nSubtiles,bool lookLeft);
	~Door();

	void Draw();

	virtual bool DoTriggerAction();
private:
	SceneState scene;
	Color color;

	Animation openL;
	Animation openR;
	Animation* currentAnim = nullptr;

	bool active;
};

#endif 
