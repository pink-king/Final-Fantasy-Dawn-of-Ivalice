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

	std::list<iPoint> entry_wall_map_positions;
	std::list<iPoint> exit_wall_map_positions;
	std::list<j1Entity*> exit_wall_entities;
};


#endif 