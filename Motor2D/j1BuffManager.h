#ifndef _J1BUFFMANAGER_H_
#define _J1BUFFMANAGER_H_

#include <list>

#include "j1Module.h"
#include "Buff.h"
#include "j1Entity.h"


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
	//take buffs to html
	void CreateBuff(BUFF_TYPE type,OBJECT_TYPE clas, std::string name, std::string character, std::string stat, float value);
	void RemoveBuff(std::string name);
	float CalculateStat(const j1Entity* ent, float initialDamage, std::string stat);
	uint GetNewSourceID();

	//combat functions
	void DirectAttack(j1Entity* attacker, j1Entity* defender, float initialDamage, std::string stat);
	bool DamageInTime(j1Entity* entity);
	//functions to create buffs in entities
	void CreateBurned(j1Entity* attacker, j1Entity* defender, float damageSecond, uint totalTime);
	void CreateParalize(j1Entity* attacker, j1Entity* defender, uint time);

	void ActiveBuff(std::string buffName, std::string character, OBJECT_TYPE clasType);
	void DeleteBuff(std::string buffName);
	float GetBurnedDamage();

private:
	uint							lastSourceID = 0u;
	std::list<Buff*>				buffs;
	pugi::xml_node					buffNode;

	float							burnedDamagesecond = 0.f;
	float							burnedTotalDamage = 0.f;

	std::list<j1Entity*>			entitiesTimeDamage;
	
};

#endif