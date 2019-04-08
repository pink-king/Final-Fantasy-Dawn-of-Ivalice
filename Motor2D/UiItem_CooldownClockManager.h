#ifndef _UIITEM_COOLDOWNCLOCKMANAGER_H_
#define _UIITEM_COOLDOWNCLOCKMANAGER_H_

#include <vector>
#include "j1Module.h"
#include "UiItem_CooldownClock.h"
#include "j1Entity.h"

class UiItem_CooldownClockManager : public j1Module
{
public:
	UiItem_CooldownClockManager();
	~UiItem_CooldownClockManager();

	bool Awake(pugi::xml_node&);  // do not know if necessary yet
	bool Start();
	bool PreUpdate() { return true; };
	bool Update(float dt);
	bool PostUpdate() { return true; };
	bool CleanUp();

	
private:
	
	UiItem_CooldownClock* ability1 = nullptr; 
	UiItem_CooldownClock* ability2 = nullptr;
	UiItem_CooldownClock* ulti = nullptr;

public: 
	
	std::vector<UiItem_CooldownClock*>			clocks;
	friend class j1Gui;
	friend class j1Scene;
};

#endif

