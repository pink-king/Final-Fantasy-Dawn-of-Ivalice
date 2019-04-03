#ifndef __J1ENTITYFACTORY_H__
#define __J1ENTITYFACTORY_H__

#include "j1Module.h"
#include "j1Entity.h"
#include "PlayerEntityManager.h"
#include <vector>

struct entityDataMap
{
	std::vector<j1Entity*> entities;

	bool isEmpty()
	{
		return entities.size() > 0;
	}
};

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
	void Debug(j1Entity* ent);
	void CreateEntitiesDataMap(int width, int height);
	bool isThisSubtileEmpty(const iPoint position) const;
	int GetSubtileEntityIndexAt(const iPoint position) const;

	std::vector<j1Entity*>* GetSubtileEntityVectorAt(const iPoint pos) const;

	static bool SortByYPos(const j1Entity* entity1, const j1Entity* entity2);

	void AssignEntityToSubtile(j1Entity* entity) const;
	bool DeleteEntityFromSubtile( j1Entity* entity) const;

//private:
	bool CheckSubtileMapBoundaries(const iPoint pos) const;

public:

	//j1Entity*				Player = nullptr;
	PlayerEntityManager*	player = nullptr;
	SDL_Texture*			texture = nullptr;
	std::vector<j1Entity*>	entities;
private:
	std::vector<j1Entity*>	draw_entities;

	// subtile data map, associated entities to subtile
	entityDataMap* entitiesDataMap = nullptr;
	int subtileWidth = 0; // stores the size in subtiles scale
	int subtileHeight = 0;

};




#endif
