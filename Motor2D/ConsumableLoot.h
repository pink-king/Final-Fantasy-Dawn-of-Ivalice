#ifndef  _CONSUMABLELOOT_H__
#define _CONSUMABLELOOT_H__

#include "LootEntity.h"


class Consumable : public LootEntity
{

public:
	Consumable(int posX, int posY);
	~Consumable();
	bool Start();
	bool Update(float dt);

	void ChooseConsumable();
	void SetConsumable();
	

	//void ConstructConsumable();
public:

	int chance;
	
	
	
	bool isUsed = false;


	//bool repositionDescription = false;
};
#endif // ! _CONSUMABLE_LOOT_H__
