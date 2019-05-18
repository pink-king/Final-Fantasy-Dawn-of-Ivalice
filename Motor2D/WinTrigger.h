#ifndef _WINTRIGGER_H_
#define _WINTRIGGER_H_

#include "Trigger.h"
#include "j1Scene.h"
#include "Color.h"

class WinTrigger : public Trigger
{
public:
	WinTrigger(float posx, float posy, SceneState scene, Color color);
	~WinTrigger();

	virtual bool DoTriggerAction();
private:
	SceneState scene;
	Color color;
};


#endif
