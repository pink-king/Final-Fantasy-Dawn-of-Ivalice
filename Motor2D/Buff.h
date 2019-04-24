#ifndef _BUFF_H_
#define _BUFF_H_

#include <vector>
#include <list>
#include <algorithm>
#include "p2Defs.h"
#include "p2Log.h"
#include "PugiXml/src/pugiconfig.hpp"
#include "PugiXml/src/pugixml.hpp"
#include "SDL/include/SDL_rect.h"
#include <string.h>
#include "j1Entity.h"

class LootEntity;

enum class BUFF_TYPE 
{
	ADDITIVE,
	MULTIPLICATIVE,
	NO_BUFF
};

enum class ELEMENTAL_TYPE
{
	NO_ELEMENT,
	FIRE_ELEMENT,
	ICE_ELEMENT,
	POISON_ELEMENT,
	STONE_ELEMENT,
	ALL_ELEMENTS,
	// only visual effects for attack manager
	DUST,
	MAX
};

enum class ROL
{
	ATTACK_ROL,
	DEFENCE_ROL,
	COOLDOWN,
	VELOCITY,
	HEALTH,
	NO_ROL
};
class Buff
{
public:
	Buff(BUFF_TYPE type, j1Entity* character, std::string stat, ELEMENTAL_TYPE elementType, ROL rol, float value, LootEntity* object = nullptr);
	~Buff();
	BUFF_TYPE GetType();
	ELEMENTAL_TYPE GetElementType();
	ROL GetRol();
	j1Entity* GetCharacter();
	LootEntity* GetItemObject();
	std::string GetStat();
	float GetValue();

	bool GetIfExist(BUFF_TYPE type, j1Entity* characer, std::string stat, ELEMENTAL_TYPE elementType, ROL rol);

private:
	LootEntity*			object = nullptr;
	j1Entity*			character = nullptr;
	BUFF_TYPE			type = BUFF_TYPE::NO_BUFF;
	std::string			stat = "\0";
	ELEMENTAL_TYPE		elementType = ELEMENTAL_TYPE::NO_ELEMENT;
	ROL			rol = ROL::NO_ROL;
	float				value = 0.F;

};


#endif