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
	bool Update(float dt) { return true; };
	bool PostUpdate() { return true; };
	bool CleanUp();

	void CreateBuff(BUFF_TYPE type, std::string name, std::string character, std::string stat, float value);
	void RemoveBuff(std::string name);
	float CalculateStat(const j1Entity* ent, float& initialDamage, std::string stat);
	uint GetNewSourceID();

	//combat functions
	void DirectAttack(j1Entity* attacker, j1Entity* defender, float initialDamage, std::string stat);
	//void ZoneAttack(j1Entity* attacker, std::vector<j1Entity*> defenders, float initialDamage);

private:
	uint lastSourceID = 0u;
	std::list<Buff*> buffs;

	pugi::xml_node buffNode;;

	
};

#endif