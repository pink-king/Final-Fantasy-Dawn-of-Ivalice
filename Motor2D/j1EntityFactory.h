#ifndef __J1ENTITYFACTORY_H__
#define __J1ENTITYFACTORY_H__

#include "j1Module.h"
#include "j1Entity.h"
#include <vector>

enum ENTITY_TYPE
{
	NO_TYPE,
	ENTITY_STATIC,
	ENTITY_DINAMIC
};

struct EntityData
{
	std::string		name;
	fPoint			position;
	ENTITY_TYPE		type;
};

class j1EntityFactory : public j1Module
{
public:
	j1EntityFactory();
	~j1EntityFactory();

	bool Awake(pugi::xml_node& node);
	bool Start();
	bool Update(float dt);
	bool PostUpdate();
	bool CleanUp();

	j1Entity* CreateEntity(ENTITY_TYPE type, int poositionX, int positionY, std::string name);
	void DestroyEntity(j1Entity* entity);

	static bool SortByYPos(const j1Entity* entity1, const j1Entity* entity2);

public:

	j1Entity*				Player;

private:
	
	SDL_Texture*			texture;
	std::vector<EntityData> entitiesInfo;
	std::vector<j1Entity*>	entities;

};




#endif
