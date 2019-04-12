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
	//by tipes of enemies
	void CreateBuff(BUFF_TYPE type, ELEMENTAL_TYPE elementType, OBJECT_ROL rol, j1Entity* character,std::string stat, float value);
	void RemoveBuff(j1Entity* character);


	float CalculateStat(const j1Entity* ent, float initialDamage, ELEMENTAL_TYPE elementType, OBJECT_ROL rol, std::string stat);

	//combat functions
	void DirectAttack(j1Entity* attacker, j1Entity* defender, float initialDamage, ELEMENTAL_TYPE elementType, std::string stat);
	bool DamageInTime(j1Entity* entity);
	//functions to create buffs in entities
	void CreateBurned(j1Entity* attacker, j1Entity* defender, float damageSecond, uint totalTime, std::string stat);
	void CreateParalize(j1Entity* attacker, j1Entity* defender, uint time);

	void ActiveBuff(std::string buffName, j1Entity* character, OBJECT_TYPE clasType);
	void DeleteBuff(Buff* buff);

	void AddItemStats(LootEntity* item);
	void RemoveItemStat(LootEntity* item);
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