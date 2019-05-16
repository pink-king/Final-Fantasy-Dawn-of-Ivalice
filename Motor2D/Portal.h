#ifndef _PORTAL_H_
#define _PORTAL_H_

#include "Trigger.h"

class Portal : public Trigger
{
public:
	Portal(float posx, float posy);
	~Portal();

	virtual bool DoTriggerAction();

};


#endif