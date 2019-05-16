#ifndef __j1TIMER_H__
#define __j1TIMER_H__

#include "p2Defs.h"

class j1Timer
{
public:

	// Constructor
	j1Timer();

	void Start();
	uint32 Read() const;
	float ReadSec() const;
	uint32 GetStartedAt();
	uint32	started_at;
private:
	
};

#endif //__j1TIMER_H__