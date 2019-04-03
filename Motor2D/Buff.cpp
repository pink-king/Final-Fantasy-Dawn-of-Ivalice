#include "Buff.h"

Buff::Buff(BUFF_TYPE type,OBJECT_TYPE object, std::string name, std::string character, std::string stat, float value) : 
	type(type),objectType(object), name(name), character(character), stat(stat), value(value)
{
}

Buff::~Buff()
{
}

BUFF_TYPE Buff::GetType()
{
	return type;
}

OBJECT_TYPE Buff::GetObjectType()
{
	return objectType;
}

std::string Buff::GetName()
{
	return name;
}

std::string Buff::GetCharacter()
{
	return character;
}

std::string Buff::GetStat()
{
	return stat;
}

float Buff::GetValue()
{
	return value;
}
