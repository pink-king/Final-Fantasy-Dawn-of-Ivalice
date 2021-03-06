#ifndef __CHESTTRIGGER_H__
#define __CHESTTRIGGER_H__

#include "Trigger.h" 

class ChestAsset;

class ChestTrigger : public Trigger
{
public:
	ChestTrigger(const fPoint& pos); 
	~ChestTrigger();

	void Draw();

	bool DoTriggerAction() override; 
	void AssignOwner(ChestAsset* owner);

private: 
	ChestAsset* myChest = nullptr;

	bool drawA = false;
};


#endif	