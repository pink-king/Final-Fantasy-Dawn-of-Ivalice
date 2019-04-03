#include "Buff.h"

Buff::Buff(BUFF_TYPE type, std::string name, std::string character, std::string stat, float value, uint source_id) : 
	type(type), name(name), character(character), stat(stat), value(value), source_id(source_id)
{
}

Buff::~Buff()
{
}

BUFF_TYPE Buff::GetType()
{
	return type;
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

uint Buff::GetSource()
{
	return source_id;
}

bool Buff::IsCausedBySource(uint source_id)
{
	return this->source_id == source_id;
}
