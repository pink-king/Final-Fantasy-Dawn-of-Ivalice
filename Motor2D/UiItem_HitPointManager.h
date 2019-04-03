#ifndef _UIITEM_HITPOINTMANAGER_H_
#define _UIITEM_HITPOINTMANAGER_H_

#include <vector>
#include "j1Module.h"
#include "UiItem_HitPoint.h"
#include "j1Entity.h"


enum damageType{
BURN,
PARALIZE,
VENOM,
STANDARD,
};

class UiItem_HitPointManager : public j1Module
{
public:
	UiItem_HitPointManager();
	~UiItem_HitPointManager();

	bool Awake(pugi::xml_node&);  // do not know if necessary yet
	bool Start();
	bool PreUpdate() { return true; };
	bool Update(float dt);
	bool PostUpdate() { return true; };
	bool CleanUp();               // TO THINK: does this need celanup? Or Gui cpp does the cleanup of all items?? That must be enough 

	//void updateHitPointPositions();   // TODO: increment all current hitpoins Y position 
	//void updateHitPointSizes();   // TODO: switch current hitpoint's states, and change size depending on the state    // use the scaling factor variable inside each hitbox
	//void updateHitPointOpacities();  // TODO: switch current hitpoint's states, and change opacity depending on the state  (fadeIn and fadeOut)


public: 
	void callHPLabelSpawn(j1Entity* enemy, uint damage, damageType type = STANDARD); // TODO: variables =  enemy pos, enemy rect top, enemy damage (buffmanager)   // TODO: call "AddHitPoint"
	//void DestroyHitPointLabel(UiItem_HitPoint*); // TODO: unload texture, and nullptr, etc

private: 
	void CheckComboTime(); 

public:
	std::vector<UiItem_HitPoint*>			hitPointLabels;   // caution: there is already a ui item list, so hitpoint will appear in both lists ????!!!!

	uint damageStreak = 0; 
	j1PerfTimer streakCountdown; 
	bool resetCountdown = false; 

	friend class j1Gui; 
};

#endif
