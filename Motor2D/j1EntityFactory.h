#ifndef __J1ENTITYFACTORY_H__
#define __J1ENTITYFACTORY_H__

#include "j1Module.h"
#include "j1Entity.h"
#include "PlayerEntityManager.h"
#include <vector>


class j1EntityFactory : public j1Module
{
public:
	j1EntityFactory();
	~j1EntityFactory();

	bool Awake(pugi::xml_node& node);
	bool Start();
	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();
	bool CleanUp();

	// entities constructors -------
	PlayerEntityManager* CreatePlayer(iPoint position);
	j1Entity* CreateEntity(ENTITY_TYPE type, int positionX, int positionY, std::string name);

	void DestroyEntity(j1Entity* entity);

	static bool SortByYPos(const j1Entity* entity1, const j1Entity* entity2);

public:

	//j1Entity*				Player = nullptr;
	PlayerEntityManager*	player = nullptr;
	SDL_Texture*			texture = nullptr;
private:
	
	
	std::vector<j1Entity*>	entities;
	std::vector<j1Entity*>	draw_entities;

};




#endif
