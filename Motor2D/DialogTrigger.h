#ifndef _VENEDORTRIGGER_H_
#define _VENEDORTRIGGER_H_

#include "Trigger.h"


class DialogTrigger : public Trigger
{
public:
	DialogTrigger(float posx, float posy,std::string Dtype);
	~DialogTrigger();

	bool Update(float dt);

	void Draw();

	bool CleanUp();

	bool Save(pugi::xml_node&) const;
	bool DoTriggerAction() override;
private:
	std::string dialogType;
	bool active = false;
	iPoint PrevSubtile = { 0,0 };
};


#endif