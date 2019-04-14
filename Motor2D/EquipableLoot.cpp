#include "EquipableLoot.h"
#include "j1Entity.h"
#include "j1LootManager.h"
#include "j1Render.h"


Equipable::Equipable(int posX, int posY) : LootEntity(LOOT_TYPE::EQUIPABLE, posX, posY)
{
	SetEquipable();

	std::vector<Buff*>::iterator stat = this->stats.begin(); 

	float attack, resistance; 

	for (; stat != this->stats.end(); ++stat)
	{

		if ((*stat)->GetRol() == OBJECT_ROL::ATTACK_ROL)
		{
			attack = (*stat)->GetValue(); 
		}
		else if ((*stat)->GetRol() == OBJECT_ROL::DEFENCE_ROL)
		{
			resistance = (*stat)->GetValue();
		}


	}
	/*if (this->equipableType == EQUIPABLE_TYPE::SWORD || this->equipableType == EQUIPABLE_TYPE::BOW || this->equipableType == EQUIPABLE_TYPE::ROD)
	{
		this->MyDescription = App->gui->AddDescriptionToWeapon(App->render->WorldToScreen(posX, posY), this->lootname, App->scene->lootPanelRect, &this->loot_rect, attack, resistance);
	}
	else
	{
		// TODO for vests, etc 
	}*/
	

	

	
}


Equipable::~Equipable()
{
	LOG("LootEquipment destoyed");
}


bool Equipable::Update(float dt)
{

	return true;
}

//bool Equipable::CleanUp()
//{
//	return true;
//}

EQUIPABLE_TYPE Equipable::ChooseEquipable()
{
	equipableChance = GetRandomValue(1, 100);

	if (equipableChance <= 33)
	{
		equipableType = EQUIPABLE_TYPE::SWORD;
		return EQUIPABLE_TYPE::SWORD;
	}
	else if (33 < equipableChance <= 66)
	{
		equipableType = EQUIPABLE_TYPE::BOW;
		return EQUIPABLE_TYPE::BOW;
	}
	else if(equipableChance > 66)
	{
		equipableType = EQUIPABLE_TYPE::ROD;
		return EQUIPABLE_TYPE::ROD;

	}

}


void Equipable::SetEquipable()
{

	switch (ChooseEquipable())
	{
	case EQUIPABLE_TYPE::SWORD:
		objectType = OBJECT_TYPE::WEAPON_OBJECT;
		loot_rect = { 59,67,16,16 };
		SetPivot(8, 12);

		size.create(16, 16);
		break;

	case EQUIPABLE_TYPE::BOW:
		objectType = OBJECT_TYPE::WEAPON_OBJECT;
		loot_rect = { 93,32,13,13 };
		SetPivot(7, 9);

		size.create(13, 13);
		break;
	case EQUIPABLE_TYPE::ROD:
		objectType = OBJECT_TYPE::WEAPON_OBJECT;
		loot_rect = { 12,67,11,16 };
		SetPivot(18, 75);

		size.create(5, 13);
		break;
	case EQUIPABLE_TYPE::ARMOR:
		objectType = OBJECT_TYPE::ARMOR_OBJECT;
		break;
	case EQUIPABLE_TYPE::VEST:
		objectType = OBJECT_TYPE::ARMOR_OBJECT;

		break;
	case EQUIPABLE_TYPE::MANTLE:
		objectType = OBJECT_TYPE::ARMOR_OBJECT;

		break;
	}
}