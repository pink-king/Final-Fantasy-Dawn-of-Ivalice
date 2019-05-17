#ifndef _NOWAWLKABLETRIGGER_H_
#define _NOWAWLKABLETRIGGER_H_

#include "Trigger.h"
#include "j1Scene.h"
#include "j1Entity.h"
#include "Color.h"
#include <vector>

class NoWalkableTrigger : public Trigger
{
public:
	NoWalkableTrigger(float posx, float posy);
	~NoWalkableTrigger();

	bool DoTriggerAction();
	void CreateWall(iPoint pos);
private:
	std::vector<j1Entity*> walls;
};


#endif