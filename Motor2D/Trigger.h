#ifndef _TRIGGER_H_
#define _TRIGGER_H_

#include "j1Entity.h"

enum class TRIGGER_TYPE
{
	PORTAL,
	LOBBYPORTAL,
	WIN,
	NOWALKABLE,
	EXITPORTAL,
	DIALOG,
	SAVE,
	NO_TRIGGER
};

class Trigger : public j1Entity
{
public:
	Trigger(TRIGGER_TYPE type, float posx, float posy,std::string name);
	~Trigger();

	virtual bool Update(float dt);

	virtual void Draw();

	bool CleanUp();

	bool Save(pugi::xml_node&) const;
	virtual bool DoTriggerAction();
	void AssignInSubtiles(int numbreOfSubtile);
public:
	TRIGGER_TYPE triggerType = TRIGGER_TYPE::NO_TRIGGER;
	SDL_Rect rect;

protected:
	int nSubtiles;
};


#endif