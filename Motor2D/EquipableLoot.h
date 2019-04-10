#ifndef _EQUIPABLELOOT_H__
#define _EQUIPABLELOOT_H__

#include "LootEntity.h"

enum class EQUIPABLE_TYPE
{
	SWORD,
	BOW,
	ROD,
	ARMOR,
	VEST,
	MANTLE
	
};

class Equipable : public LootEntity
{
public:
	Equipable(int posX, int posY);
	~Equipable();

	bool Update(float dt);
//	bool CleanUp();

	void ChooseEquipable();
	void SetEquipable();
	void Draw();
public:

	EQUIPABLE_TYPE equipable_type;

	int id;
	int itemLevel;

	std::string character;
	std::string itemName;


	float dmg;
	float burn;
	float cooldown;
	float dmg_back;
	float attck_spd;
	float slow;
	float poisonBack;

	int equipableChance;


};

#endif // !_EQUIPABLELOOT_H__
