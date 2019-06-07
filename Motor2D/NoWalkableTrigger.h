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
	Entry,
};

struct wall
{
	DOOR doorType;
	j1Entity* entity = nullptr;
};

class NoWalkableTrigger : public Trigger
{
public:
	NoWalkableTrigger(float posx, float posy, bool bossHall, uint nSubtiles);
	~NoWalkableTrigger();

	bool DoTriggerAction();
	void CreateExitWall(iPoint pos);
	void CreateEntryWall(iPoint pos);

	void CreateWalls();

	bool Update(float dt) override;

private:
	std::vector<wall*> wallsvec;
	bool isActivated = false;
	bool bossHall;
	uint level = 1;

	j1Entity* waveEntity = nullptr;

	SDL_Rect waveZone;

	std::list<iPoint> entry_wall_map_positions;
	std::list<iPoint> exit_wall_map_positions;
	std::list<j1Entity*> exit_wall_entities;
};


#endif 