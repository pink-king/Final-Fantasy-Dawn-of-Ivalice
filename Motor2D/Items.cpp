#include "Items.h"
#include "j1EntityFactory.h"

items::items(std::string name, OBJECT_TYPE type):name(name),type(type)
{
//	this->stats.swap(stats);
}

items::~items()
{
	std::vector<Buff*>::iterator item = stats.begin();
	for (; item != stats.end(); ++item)
	{
		delete (*item);
		(*item) = nullptr;
	}
	stats.clear();
}

std::string items::GetName()
{
	return name;
}

OBJECT_TYPE items::GetType()
{
	return type;
}

void items::CreateBuff(BUFF_TYPE type, std::string character, ELEMENTAL_TYPE elementType, OBJECT_ROL rol, float value)
{
	stats.push_back(new Buff(type, character, elementType, rol, value));
}

