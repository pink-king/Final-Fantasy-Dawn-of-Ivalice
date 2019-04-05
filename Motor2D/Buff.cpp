#include "Buff.h"

Buff::Buff(BUFF_TYPE type, std::string character, ELEMENTAL_TYPE elementType, OBJECT_ROL rol, float value) :
	type(type), character(character), elementType(elementType), rol(rol), value(value)
{
}

Buff::~Buff()
{
}

BUFF_TYPE Buff::GetType()
{
	return type;
}

std::string Buff::GetCharacter()
{
	return character;
}

ELEMENTAL_TYPE Buff::GetElementType()
{
	return elementType;
}

OBJECT_ROL Buff::GetRol()
{
	return rol;
}

float Buff::GetValue()
{
	return value;
}

bool Buff::GetIfExist(BUFF_TYPE type, std::string character, ELEMENTAL_TYPE elementType, OBJECT_ROL rol)
{
	return (this->type == type && this->character.compare(character) && this->elementType == elementType && this->rol == rol);
}
