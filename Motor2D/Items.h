#ifndef _ITEMS_H_
#define _ITEMS_H_

#include "Buff.h"

enum class OBJECT_TYPE
{
	WEAPON_OBJECT,
	ARMOR_OBJECT,
	HEAD_OBJECT,
	NO_OBJECT
};

class items
{
public:
	items(std::string name, OBJECT_TYPE type);
	~items();

	std::string GetName();
	OBJECT_TYPE GetType();
	void CreateBuff(BUFF_TYPE type, std::string character, ELEMENTAL_TYPE elementType, OBJECT_ROL rol, float value);

private:
	std::string			name;
	OBJECT_TYPE			type;

public:
	std::vector<Buff*>	stats;

};

#endif
