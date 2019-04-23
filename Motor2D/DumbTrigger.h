#ifndef __DUMBTRIGGER_H__
#define __DUMBTRIGGER_H__

#include "j1Entity.h"

class DumbTrigger : public j1Entity
{
public: 

	DumbTrigger(iPoint pos);
	~DumbTrigger(); 

private: 

	iPoint mySubtile;
	iPoint adjacents[11]; 
};

#endif