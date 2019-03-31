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

enum class OBJECT_TYPE
{
	WEAPON_OBJECT,
	ARMOR_OBJECT,
	HEAD_OBJECT,
	NO_OBJECT

};

class Buff
{
public:
	Buff(BUFF_TYPE type, OBJECT_TYPE object, std::string name, std::string characer, std::string stat, float value, uint source_id);
	~Buff();
	BUFF_TYPE GetType();
	OBJECT_TYPE GetObjectType();
	std::string GetName();
	std::string GetCharacter();
	std::string GetStat();
	float GetValue();
	uint GetSource();
	bool IsCausedBySource(uint source_id);
	bool GetIfActive();
	void DisactiveBuff();
	void ActiveBuff();

private:
	BUFF_TYPE			type = BUFF_TYPE::NO_BUFF;
	OBJECT_TYPE			objectType = OBJECT_TYPE::NO_OBJECT;
	std::string			name = "\0";
	std::string			character = "\0";
	std::string			stat = "\0";
	float				value = 0.f;// If the item have 2 values create 2 buff with similar name
	uint				source_id = 0u;//ID from which modifier (object, spell, etc) the buff came from
	bool				isActive = true;
};


#endif