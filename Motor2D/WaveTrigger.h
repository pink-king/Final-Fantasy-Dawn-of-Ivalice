#ifndef _WAVETRIGGER_H_
#define _WAVETRIGGER_H_

#include "Trigger.h"
#include "j1Scene.h"
#include "j1Entity.h"
#include "Color.h"
#include <vector>
#include "NoWalkableTrigger.h"

class WaveTrigger : public Trigger
{
public:
	WaveTrigger(float posx, float posy, uint level);
	~WaveTrigger();

	bool DoTriggerAction();
	void CreateExitWall(iPoint pos);
	void CreateEntryWall(iPoint pos);

	void CreateWalls();

private:
	std::vector<wall*> wallsvec;
	bool isActivated = false;

	uint level = 1; 
};


#endif 