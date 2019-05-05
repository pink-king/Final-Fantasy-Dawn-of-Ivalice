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

void Buff::SetCharacter(j1Entity * newCharacter)
{
	character = newCharacter;
}

LootEntity * Buff::GetItemObject()
{
	return object;
}

void Buff::SetItemObject(LootEntity * newObject)
{
	object = newObject;
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

Buff* Buff::Load(pugi::xml_node &node)
{
	pugi::xml_node nodeData = node.child("data");

	std::string charType = nodeData.attribute("type").as_string();
	if (charType.compare("additive") == 0)
		type = BUFF_TYPE::ADDITIVE;
	else if (charType.compare("multiplicative") == 0)
		type = BUFF_TYPE::MULTIPLICATIVE;

	std::string charElement = nodeData.attribute("elementType").as_string();
	if (charElement.compare("noElement") == 0)
		elementType = ELEMENTAL_TYPE::NO_ELEMENT;
	else if (charElement.compare("fireElement") == 0)
		elementType = ELEMENTAL_TYPE::FIRE_ELEMENT;
	else if (charElement.compare("iceElement") == 0)
		elementType = ELEMENTAL_TYPE::ICE_ELEMENT;
	else if (charElement.compare("poisonElement") == 0)
		elementType = ELEMENTAL_TYPE::POISON_ELEMENT;
	else if (charElement.compare("stoneElement") == 0)
		elementType = ELEMENTAL_TYPE::STONE_ELEMENT;
	else if (charElement.compare("allElement") == 0)
		elementType = ELEMENTAL_TYPE::ALL_ELEMENTS;
	else if (charElement.compare("dust") == 0)
		elementType = ELEMENTAL_TYPE::DUST;

	std::string charRol = nodeData.attribute("rol").as_string();
	if (charRol.compare("attack") == 0)
		rol = ROL::ATTACK_ROL;
	else if (charRol.compare("defence") == 0)
		rol = ROL::DEFENCE_ROL;
	else if (charRol.compare("cooldown") == 0)
		rol = ROL::COOLDOWN;
	else if (charRol.compare("velocity") == 0)
		rol = ROL::VELOCITY;
	else if (charRol.compare("health") == 0)
		rol = ROL::HEALTH;

	value = nodeData.attribute("value").as_float();
	return this;
}

bool Buff::Save(pugi::xml_node &node) const
{
	pugi::xml_node nodeData = node.append_child("data");

	nodeData.append_attribute("character") = character->name.data();

	switch (type)
	{
	case BUFF_TYPE::ADDITIVE:
		nodeData.append_attribute("type") = "additive";
		break;
	case BUFF_TYPE::MULTIPLICATIVE:
		nodeData.append_attribute("type") = "multiplicative";
		break;
	case BUFF_TYPE::NO_BUFF:
		break;
	default:
		break;
	}

	switch (elementType)
	{
	case ELEMENTAL_TYPE::NO_ELEMENT:
		nodeData.append_attribute("elementType") = "noElement";
		break;
	case ELEMENTAL_TYPE::FIRE_ELEMENT:
		nodeData.append_attribute("elementType") = "fireElement";
		break;
	case ELEMENTAL_TYPE::ICE_ELEMENT:
		nodeData.append_attribute("elementType") = "iceElement";
		break;
	case ELEMENTAL_TYPE::POISON_ELEMENT:
		nodeData.append_attribute("elementType") = "poisonElement";
		break;
	case ELEMENTAL_TYPE::STONE_ELEMENT:
		nodeData.append_attribute("elementType") = "stoneElement";
		break;
	case ELEMENTAL_TYPE::ALL_ELEMENTS:
		nodeData.append_attribute("elementType") = "allElement";
		break;
	case ELEMENTAL_TYPE::DUST:
		nodeData.append_attribute("elementType") = "dust";
		break;
	case ELEMENTAL_TYPE::MAX:
		break;
	default:
		break;
	}

	switch (rol)
	{
	case ROL::ATTACK_ROL:
		nodeData.append_attribute("rol") = "attack";
		break;
	case ROL::DEFENCE_ROL:
		nodeData.append_attribute("rol") = "defence";
		break;
	case ROL::COOLDOWN:
		nodeData.append_attribute("rol") = "cooldown";
		break;
	case ROL::VELOCITY:
		nodeData.append_attribute("rol") = "velocity";
		break;
	case ROL::HEALTH:
		nodeData.append_attribute("rol") = "health";
		break;
	case ROL::NO_ROL:
		break;
	default:
		break;
	}

	nodeData.append_attribute("value") = value;

	return true;
}

bool Buff::GetIfExist(BUFF_TYPE type, j1Entity* character, std::string stat, ELEMENTAL_TYPE elementType, ROL rol)
{
	return (this->type == type && this->character == character && this->elementType == elementType && this->rol == rol && this->stat == stat);
}


//bool Buff::IsCausedBySource(uint source_id)
//{
//	return this->source_id == source_id;
//}