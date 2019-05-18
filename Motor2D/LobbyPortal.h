#ifndef _LOBBYPORTAL_H_
#define _LOBBYPORTAL_H_

#include "Trigger.h"
#include "j1Scene.h"
#include "Color.h"

class LobbyPortal : public Trigger
{
public:
	LobbyPortal(float posx, float posy, SceneState scene, Color color);
	~LobbyPortal();

	virtual bool DoTriggerAction();
private:
	SceneState scene;
	Color color;
};


#endif