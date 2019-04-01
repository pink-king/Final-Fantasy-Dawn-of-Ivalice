#ifndef __j1ATTACKMANAGER_H__
#define __j1ATTACKMANAGER_H__

#include <queue>
#include <vector>
#include "j1Module.h"
#include "p2Point.h"
#include "j1Timer.h"
#include "j1Entity.h"

class j1Entity;

enum class propagationType
{
	BFS,
	HEURISTIC
};

class attackData // intelligent class that manage itsel from the attackmanager calls
{
public:
	attackData();
	attackData(j1Entity* fromEntity, iPoint startSubtilePoint, propagationType propaType, int baseDamage, int subtileRadius, uint32 propagationStepSpeed); // all time relative are on ms
	~attackData();

	bool Start();
	bool Update(float dt);
	bool PostUpdate();

protected:
	bool DoNextPropagationStep();
	bool DoNextStepBFS();
	//bool DoNextStepHeuristic();
	
public:
	bool to_erase = false;
private:
	bool debug = true;
	std::vector<j1Entity*> GetInvoldedEntitiesFromSubtile(const iPoint subtilePos);
	// propagation data itself -----------
	iPoint startSubtilePoint;
	propagationType propaType;
	int baseDamage;
	int subTileStepRadius; // stores the max expansion "radius" on subtile units
	int currentPropagationStep; // stores the current step
	uint32 propagationStepSpeed; // stores the speed to switch to next step of propagation state
	j1Timer stepTimer;
	// associated entity
	//j1Entity* fromEntity = nullptr;
	std::queue<iPoint> frontier;
	std::vector<iPoint> visited;

};

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

	// functionality
	void AddPropagationAttack(iPoint startSubtilePoint, propagationType propagationType, int baseDamage, int subtileStepRadius, uint32 propagationStepSpeed);
	void RemovePropagationAttack(attackData* attackDataPackage);


public:
	SDL_Texture* debugSubtileTex = nullptr;
	SDL_Texture* debugSubtileTex2 = nullptr;

private:
	std::vector<attackData*> currentPropagationAttacks;
	//bool debug = true;
	
};

#endif // __j1ATTACKMANAGER_H__