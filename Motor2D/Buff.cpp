#include "Buff.h"
#include "LootEntity.h"

Buff::Buff(BUFF_TYPE type, j1Entity* character, std::string stat, ELEMENTAL_TYPE elementType, ROL rol, float value, LootEntity* object) :
	type(type), character(character), stat(stat), elementType(elementType), rol(rol), value(value), object(object)
{
}

//Buff::Buff(BUFF_TYPE type, std::string name, std::string character, std::string stat, float value, uint source_id) :
//	type(type), name(name), character(character), stat(stat), value(value), source_id(source_id)
//{
//}

Buff::~Buff()
{
	object = nullptr;
	character = nullptr;
}

BUFF_TYPE Buff::GetType()
{
	return type;
}

j1Entity* Buff::GetCharacter()
{
	return character;
}

LootEntity * Buff::GetItemObject()
{
	return object;
}

std::string Buff::GetStat()
{
	return stat;
}

ELEMENTAL_TYPE Buff::GetElementType()
{
	return elementType;
}

ROL Buff::GetRol()
{
	return rol;
}

float Buff::GetValue()
{
	return value;
}

bool Buff::GetIfExist(BUFF_TYPE type, j1Entity* character, std::string stat, ELEMENTAL_TYPE elementType, ROL rol)
{
	return (this->type == type && this->character == character && this->elementType == elementType && this->rol == rol && this->stat == stat);
}


//bool Buff::IsCausedBySource(uint source_id)
//{
//	return this->source_id == source_id;
//}