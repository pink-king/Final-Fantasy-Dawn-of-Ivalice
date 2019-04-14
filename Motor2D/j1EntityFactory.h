#ifndef __J1ENTITYFACTORY_H__
#define __J1ENTITYFACTORY_H__

#include "j1Module.h"
#include "j1Entity.h"
#include "Enemy.h"
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
	Enemy* CreateEnemy(EnemyType etype, iPoint pos, uint speed, uint tilesDetectionRange, uint attackRange, uint baseDamage, float attackSpeed);
	void Debug(j1Entity* ent);
	void CreateEntitiesDataMap(int width, int height);
	bool isThisSubtileEmpty(const iPoint position) const;
	bool isThisSubtileEnemyFree(const iPoint pos) const; 
	int GetSubtileEntityIndexAt(const iPoint position) const;
	bool areAllSubtilesReserved() const; 

	std::vector<j1Entity*>* GetSubtileEntityVectorAt(const iPoint pos) const;

	static bool SortByYPos(const j1Entity* entity1, const j1Entity* entity2);

	void AssignEntityToSubtile(j1Entity* entity) const;
	bool DeleteEntityFromSubtile( j1Entity* entity) const;

	// Subyacent subtiles functions

	bool isPlayerAdjacent(const iPoint& pos) const; 
	iPoint TranslateToRelativePlayerPos(const iPoint& pos) const; 
	int GetAdjacentIndex(const iPoint& pos) const; 
	void ReserveAdjacent(const iPoint& pos);
	void FreeAdjacent(const iPoint& pos);
	bool isThisSubtileReserved(const iPoint& pos) const;
	void ReleaseAllReservedSubtiles();
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
	bool reservedAdjacentSubtiles[9];

	int subtileWidth = 0; // stores the size in subtiles scale
	int subtileHeight = 0;

};




#endif
