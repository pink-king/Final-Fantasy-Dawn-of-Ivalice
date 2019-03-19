#ifndef __PLAYERENTITYMANAGER_H__
#define __PLAYERENTITYMANAGER_H__

#include "j1Module.h"
#include "j1EntityFactory.h"
#include <vector>


class PlayerEntityManager : public j1Entity
{
public:
	PlayerEntityManager();
	~PlayerEntityManager();

	//core loops ------
	//bool Awake(pugi::xml_node& node);
	bool Start();
	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();
	bool CleanUp();

	// functionality ------
	bool SwapInputChecker();

public:

	//j1Entity*				Player = nullptr;

private:

	SDL_Texture*			texture = nullptr;
	/*std::vector<j1Entity*>	entities;
	std::vector<j1Entity*> draw_entities;*/

};

#endif