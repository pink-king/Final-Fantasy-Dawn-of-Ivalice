#ifndef _J1BUFFMANAGER_H_
#define _J1BUFFMANAGER_H_

#include <list>

#include "j1Module.h"
#include "Buff.h"
#include "j1Entity.h"
#include "Items.h"


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
	void CreateEnemyBuff(BUFF_TYPE type, ELEMENTAL_TYPE elementType, OBJECT_ROL rol, std::string character,std::string stat, float value);
	void RemoveEnemyBuff(std::string character);


	float CalculateStat(const j1Entity* ent, float initialDamage, ELEMENTAL_TYPE elementType, OBJECT_ROL rol, std::string stat);

	//combat functions
	void DirectAttack(j1Entity* attacker, j1Entity* defender, float initialDamage, ELEMENTAL_TYPE elementType, std::string stat);
	bool DamageInTime(j1Entity* entity);
	//functions to create buffs in entities
	void CreateBurned(j1Entity* attacker, j1Entity* defender, float damageSecond, uint totalTime, std::string stat);
	void CreateParalize(j1Entity* attacker, j1Entity* defender, uint time);

	void ActiveBuff(std::string buffName, std::string character, OBJECT_TYPE clasType);
	void DeleteBuff(Buff* buff);

	void AddItemStats(items* item);
	void RemoveItemStat(items* item);
	iPoint enemydeadsubtile;
private:
	std::list<Buff*>				buffs;
	pugi::xml_node					buffNode;

	std::list<j1Entity*>			entitiesTimeDamage;
	
};

#endif