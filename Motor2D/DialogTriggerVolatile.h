#ifndef __DIALOGTRIGGERVOLATILE_H__
#define __DIALOGTRIGGERVOLATILE_H__


#include "Trigger.h"

class DialogTriggerVolatile : public Trigger
{
public:

	DialogTriggerVolatile(float posx, float posy, std::string Dtype, uint nSubtiles, DialogTriggerVolatile* parent = nullptr);
	~DialogTriggerVolatile();

	bool PreUpdate() override; 
	bool DoTriggerAction() override;

	void CheckParentDestroyed();

private: 

	DialogTriggerVolatile* parent = nullptr; 
	std::string dialogType; 
};



#endif // __DIALOGTIGGERVOLATILE_H__
