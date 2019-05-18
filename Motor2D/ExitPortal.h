#ifndef _EXITPORTAL_H_
#define _EXITPORTAL_H_

#include "Trigger.h"
#include "j1Scene.h"
#include "Color.h"

class ExitPortal : public Trigger
{
public:
	ExitPortal(float posx, float posy);
	~ExitPortal();

	bool Update(float dt);

	void Draw();

	virtual bool DoTriggerAction();
private:

	Animation close;
	Animation idle;

	Animation* currentAnim = nullptr;
	bool doit = false;
	j1Timer timer;

};

#endif 