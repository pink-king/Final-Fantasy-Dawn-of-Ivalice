#ifndef _UIITEM_HITPOINTMANAGER_H_
#define _UIITEM_HITPOINTMANAGER_H_

#include <vector>
#include "j1Module.h"
#include "UiItem_HitPoint.h"
#include "Buff.h"



struct labelsSpawned
{
	bool fierce = false; 
	bool brutal = false; 
	uint totalLabels = 0; 
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
	bool CleanUp();              


public: 
	UiItem_HitPoint* callHPLabelSpawn(iPoint pos, uint damage, ELEMENTAL_TYPE type = ELEMENTAL_TYPE::NORMAL_ELEMENT);
	void calculatePlayerCombo(); 


public:
	std::vector<UiItem_HitPoint*>			hitPointLabels;   
	uint		                            labelScoreAccum;
	uint                                    playerStreak = 0; 
	labelsSpawned                           labelsSpawned; 
	friend class j1Gui; 
};

#endif
