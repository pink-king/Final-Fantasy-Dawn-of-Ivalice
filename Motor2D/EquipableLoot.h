#ifndef _EQUIPABLELOOT_H__
#define _EQUIPABLELOOT_H__

#include "LootEntity.h"
#include"j1Timer.h"


class Equipable : public LootEntity
{
public:
	Equipable(int posX, int posY);
	Equipable(int posX, int posY, EQUIPABLE_TYPE OBJ_TYPE);

	~Equipable();

	bool Update(float dt);
//	bool CleanUp();

	EQUIPABLE_TYPE ChooseEquipable();
	void SetEquipable();
	void ToSelectLootFunction(EQUIPABLE_TYPE type);

public:

	

	int id;
	std::string character;

	int equipableChance;

	//bool repositionDescription = false; 
};

#endif // !_EQUIPABLELOOT_H__
