#ifndef _DIALOGTRIGGER_H_
#define _DIALOGTRIGGER_H_

#include "Trigger.h"


class DialogTrigger : public Trigger
{
public:
	DialogTrigger(float posx, float posy,std::string Dtype,iPoint posState, uint nSubtiles, bool pressA);
	~DialogTrigger();

	bool Update(float dt);

	void Draw();

	bool CleanUp();

	bool Save(pugi::xml_node&) const;
	bool DoTriggerAction() override;
private:
	std::string dialogType;
	bool pressA = false;
	iPoint PrevSubtile = { 0,0 };
	iPoint posState;
	Animation dialogSign;
	Animation onTrigger;
	Animation* currentAnim = nullptr;

	bool active = false;
};


#endif