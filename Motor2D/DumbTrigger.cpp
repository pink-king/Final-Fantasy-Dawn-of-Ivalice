#include "DumbTrigger.h"
#include "j1EntityFactory.h"
#include "j1Map.h"

DumbTrigger::DumbTrigger(iPoint pos) : j1Entity(pos, { 0,0,0,0 }) 
{
	mySubtile = App->map->WorldToSubtileMap(pos.x, pos.y);
	adjacents[0] = { mySubtile.x - 1, mySubtile.y - 1 };
	adjacents[1] = { mySubtile.x, mySubtile.y - 1};
	adjacents[2] = { mySubtile.x + 1, mySubtile.y - 1 };

	adjacents[3] = { mySubtile.x - 1, mySubtile.y};
	//adjacents[4] = { mySubtile.x, mySubtile.y };	// NOT TO BE USED
	adjacents[4] = { mySubtile.x + 1, mySubtile.y };

	adjacents[5] = { mySubtile.x - 1, mySubtile.y +1};
	adjacents[6] = { mySubtile.x, mySubtile.y + 1 };
	adjacents[7] = { mySubtile.x + 1, mySubtile.y + 1 };


	adjacents[8] = { mySubtile.x + 2, mySubtile.y + 1 };
	adjacents[9] = { mySubtile.x + 2, mySubtile.y };
	adjacents[10] = { mySubtile.x + 2, mySubtile.y - 1 };

	for (uint i = 0; i <= 10; i++)
	{
		App->entityFactory->AssignEntityToSubtilePos(this, adjacents[i]);
	}
	//App->entityFactory->AssignEntityToSubtilePos(this, { mySubtile.x, mySubtile.y }); MY SUBTILE xd

	/*App->entityFactory->AssignEntityToSubtilePos(this, { mySubtile.x +1, mySubtile.y + 1});
	App->entityFactory->AssignEntityToSubtilePos(this, { mySubtile.x, mySubtile.y + 1});
	App->entityFactory->AssignEntityToSubtilePos(this, { mySubtile.x -1, mySubtile.y + 1});
	App->entityFactory->AssignEntityToSubtilePos(this, { mySubtile.x +1, mySubtile.y });
	App->entityFactory->AssignEntityToSubtilePos(this, { mySubtile.x -1, mySubtile.y });
	App->entityFactory->AssignEntityToSubtilePos(this, { mySubtile.x -1, mySubtile.y -1});
	App->entityFactory->AssignEntityToSubtilePos(this, { mySubtile.x - 1, mySubtile.y });
	App->entityFactory->AssignEntityToSubtilePos(this, { mySubtile.x - 1, mySubtile.y +1});*/
	type = TRIGGERWIN;

	SetPivot(0, 0);
}


DumbTrigger::~DumbTrigger()
{

	for (uint i = 0; i <= 10; i++)
	{
		App->entityFactory->DeleteEntityFromSubtilePos(this, adjacents[i]);
	}

	//App->entityFactory->DeleteEntityFromSubtilePos(this, { mySubtile.x, mySubtile.y }); MY SUBTILE xd
	/*App->entityFactory->DeleteEntityFromSubtilePos(this, { mySubtile.x + 1, mySubtile.y + 1 });
	App->entityFactory->DeleteEntityFromSubtilePos(this, { mySubtile.x, mySubtile.y + 1 });
	App->entityFactory->DeleteEntityFromSubtilePos(this, { mySubtile.x - 1, mySubtile.y + 1 });
	App->entityFactory->DeleteEntityFromSubtilePos(this, { mySubtile.x + 1, mySubtile.y });
	App->entityFactory->DeleteEntityFromSubtilePos(this, { mySubtile.x - 1, mySubtile.y });
	App->entityFactory->DeleteEntityFromSubtilePos(this, { mySubtile.x - 1, mySubtile.y - 1 });
	App->entityFactory->DeleteEntityFromSubtilePos(this, { mySubtile.x - 1, mySubtile.y });
	App->entityFactory->DeleteEntityFromSubtilePos(this, { mySubtile.x - 1, mySubtile.y + 1 });*/

}
