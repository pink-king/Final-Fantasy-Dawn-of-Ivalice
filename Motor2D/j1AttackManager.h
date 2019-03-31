#ifndef __j1ATTACKMANAGER_H__
#define __j1ATTACKMANAGER_H__

#include "j1Module.h"


class j1AttackManager : public j1Module
{
public:

	j1AttackManager();
	~j1AttackManager();

	// Called before render is available
	bool Awake(pugi::xml_node&);
	bool Start();
	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();


	// Called before quitting
	bool CleanUp();

	

private:

	
};

#endif // __j1ATTACKMANAGER_H__