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

enum class BUFF_TYPE 
{
	ADDITIVE,
	MULTIPLICATIVE,
	NO_BUFF
};

enum class ELEMENTAL_TYPE
{
	FIRE_ELEMENT,
	ICE_ELEMENT,
	POISON_ELEMENT,
	NORMAL_ELEMENT
};

enum class OBJECT_ROL
{
	ATTACK_ROL,
	DEFENCE_ROL,
	NO_ROL
};
class Buff
{
public:
	Buff(BUFF_TYPE type, std::string character, std::string stat, ELEMENTAL_TYPE elementType, OBJECT_ROL rol, float value);
	~Buff();
	BUFF_TYPE GetType();
	ELEMENTAL_TYPE GetElementType();
	OBJECT_ROL GetRol();
	std::string GetCharacter();
	std::string GetStat();
	float GetValue();

	bool GetIfExist(BUFF_TYPE type, std::string characer, std::string stat, ELEMENTAL_TYPE elementType, OBJECT_ROL rol);

private:
	BUFF_TYPE			type = BUFF_TYPE::NO_BUFF;
	ELEMENTAL_TYPE		elementType = ELEMENTAL_TYPE::NORMAL_ELEMENT;
	OBJECT_ROL			rol = OBJECT_ROL::NO_ROL;
	std::string			character = "\0";
	std::string			stat = "\0";
	float				value = 0.f;


};


#endif