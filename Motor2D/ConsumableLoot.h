#ifndef  _CONSUMABLELOOT_H__
#define _CONSUMABLELOOT_H__

#include "LootEntity.h"

enum class CONSUMABLE_TYPE
{
	POTION,
	NO_CONSUMABLE
};

class Consumable : public LootEntity
{

public:
	Consumable(int posX, int posY);
	Consumable(int posX, int posY, OBJECT_TYPE objectT, CONSUMABLE_TYPE consumableT);
	~Consumable();
	bool Start();
	bool Update(float dt);

	void ChooseConsumable(CONSUMABLE_TYPE type = CONSUMABLE_TYPE::NO_CONSUMABLE);
	void SetConsumable(CONSUMABLE_TYPE type = CONSUMABLE_TYPE::NO_CONSUMABLE);
	

	//void ConstructConsumable();
public:
	CONSUMABLE_TYPE  consumableType;
	int chance;
	
	
	
	bool isUsed = false;


	//bool repositionDescription = false;
};
#endif // ! _CONSUMABLE_LOOT_H__
