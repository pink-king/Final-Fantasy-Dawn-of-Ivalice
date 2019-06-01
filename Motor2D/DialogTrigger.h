#ifndef _DIALOGTRIGGER_H_
#define _DIALOGTRIGGER_H_

#include "Trigger.h"


class DialogTrigger : public Trigger
{
public:
	DialogTrigger(float posx, float posy,std::string Dtype, uint nSubtiles, bool pressA);
	~DialogTrigger();

	bool Update(float dt);

	bool CleanUp();

	bool Save(pugi::xml_node&) const;
	bool DoTriggerAction() override;
private:
	std::string dialogType;
	bool pressA = false;
	iPoint PrevSubtile = { 0,0 };
};


#endif