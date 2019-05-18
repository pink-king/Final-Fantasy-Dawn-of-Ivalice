#ifndef _NOWAWLKABLETRIGGER_H_
#define _NOWAWLKABLETRIGGER_H_

#include "Trigger.h"
#include "j1Scene.h"
#include "j1Entity.h"
#include "Color.h"
#include <vector>

enum class DOOR
{
	Exit,
	entry,
};

struct wall
{
	DOOR doorType;
	j1Entity* entity = nullptr;
};

class NoWalkableTrigger : public Trigger
{
public:
	NoWalkableTrigger(float posx, float posy);
	~NoWalkableTrigger();

	bool DoTriggerAction();
	void CreateExitWall(iPoint pos);
	void CreateEntryWall(iPoint pos); 

private:
	std::vector<wall*> walls;
	bool isActived = false;
};


#endif 