#ifndef _J1BUFFMANAGER_H_
#define _J1BUFFMANAGER_H_

#include <list>

#include "j1Module.h"
#include "Buff.h"
#include "LootEntity.h"
#include "UiItem_HitPointManager.h"


class j1BuffManager : public j1Module 
{
public:
	j1BuffManager();
	~j1BuffManager();

	bool Awake(pugi::xml_node&);
	bool Start();
	bool PreUpdate() { return true; };
	bool Update(float dt);
	bool PostUpdate() { return true; };
	bool CleanUp();
	//by buff without items
	void CreateBuff(BUFF_TYPE type, ELEMENTAL_TYPE elementType, ROL rol, j1Entity* character,std::string stat, float value);
	//for delete all buff of one entity
	void RemoveBuff(j1Entity* character);

	float CalculateStat(const j1Entity* ent, float initialDamage, ELEMENTAL_TYPE elementType, ROL rol, std::string stat);

	//combat functions
	void DirectAttack(j1Entity* attacker, j1Entity* defender, float initialDamage, ELEMENTAL_TYPE elementType, std::string stat);
	bool DamageInTime(j1Entity* entity);
	//functions to create damage in time
	void CreateBurned(j1Entity* attacker, j1Entity* defender, float damageSecond, uint totalTime, std::string stat);
	void CreateParalize(j1Entity* attacker, j1Entity* defender, uint time);
	void CreateHealth(j1Entity* entity, float lifeSecond, uint time);

	void ChangeEntityVariables(j1Entity* entity, BUFF_TYPE type, ROL rol, float value);
	void ResetEntityVariables(Buff* buff);
	//delete one buff
	void DeleteBuff(Buff* buff);

	void AddItemStats(LootEntity* item);
	void RemoveItemStat(const LootEntity* item);
	iPoint enemydeadsubtile;
private:
	uint							lastSourceID = 0u;
	std::list<Buff*>				buffs;
	pugi::xml_node					buffNode;

	std::list<j1Entity*>			entitiesTimeDamage;
	
public:
	float powerAttack = 5;
};

#endif