#ifndef _EQUIPABLELOOT_H__
#define _EQUIPABLELOOT_H__

#include "LootEntity.h"

//enum class EQUIPABLE_TYPE
//{
//	SWORD,
//	BOW,
//	ROD,
//	ARMOR,
//	VEST,
//	MANTLE
//	
//};

class Equipable : public LootEntity
{
public:
	Equipable(int posX, int posY);
	~Equipable();

	bool Update(float dt);
//	bool CleanUp();

	EQUIPABLE_TYPE ChooseEquipable();
	void SetEquipable();
	
public:

	

	int id;
	std::string character;

	int equipableChance;
	bool repositionDescription = false;

};

#endif // !_EQUIPABLELOOT_H__
