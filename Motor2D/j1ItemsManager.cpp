#include "j1ItemsManager.h"
#include "j1App.h"
#include "j1EntityFactory.h"
#include "j1BuffManager.h"

//j1ItemsManager::j1ItemsManager()
//{
//	name.assign("ItemsManager");
//}
//
//j1ItemsManager::~j1ItemsManager()
//{
//}
//
//bool j1ItemsManager::Awake(pugi::xml_node &config)
//{
//	node = config;
//	return true;
//}
//
//bool j1ItemsManager::Start()
//{
//	return true;
//}
//
//bool j1ItemsManager::Update(float dt)
//{
//	return true;
//}
//
//bool j1ItemsManager::CleanUp()
//{
//	return true;
//}
//
//void j1ItemsManager::CreateItem(std::string name, OBJECT_TYPE type, j1Entity* entity)
//{
//	items* object = nullptr;
//	if (entity->equipedObjects.size() == 0)
//	{
//		object = new items(name, type);
//		LoadItemData(object);
//		entity->equipedObjects.push_back(object);
//		App->buff->AddItemStats(object);
//	}
//	else
//	{
//		bool existSimilar = false;
//		std::vector<items*>::iterator item = entity->equipedObjects.begin();
//		for (; item != entity->equipedObjects.end(); ++item)
//		{
//
//			if (type == (*item)->GetType())
//				existSimilar = true;
//		}
//
//		if (!existSimilar)
//		{
//			object = new items(name, type);
//			LoadItemData(object);
//			entity->equipedObjects.push_back(object);
//			App->buff->AddItemStats(object);
//		}
//		else 
//		{
//			object = new items(name, type);
//			LoadItemData(object);
//			entity->equipedObjects.push_back(object);
//		}
//	}
//}
//
//void j1ItemsManager::RemoveItem(j1Entity* entity, std::string name)
//{
//	if (entity->bagObjects.size() != 0)
//	{
//		std::vector<items*>::iterator iter = entity->bagObjects.begin();
//		for (; iter != entity->bagObjects.end(); ++iter)
//		{
//			if (name.compare((*iter)->GetName()) == 0)
//			{
//				entity->bagObjects.erase(iter);
//				break;
//			}
//		}
//	}
//	if (entity->equipedObjects.size() != 0)
//	{
//		std::vector<items*>::iterator iter2 = entity->equipedObjects.begin();
//		for (; iter2 != entity->equipedObjects.end(); ++iter2)
//		{
//			if (name.compare((*iter2)->GetName()) == 0)
//			{
//				App->buff->RemoveItemStat(*iter2);
//				entity->equipedObjects.erase(iter2);
//				break;
//			}
//		}
//	}
//}
//
//void j1ItemsManager::EquipItem(j1Entity* entity, items * item)
//{
//	DesequipSimilarItem(entity, item);
//	items* transferItem = nullptr;
//	std::vector<items*>::iterator iter = entity->bagObjects.begin();
//	for (; iter != entity->bagObjects.end(); ++iter)
//	{
//		if (item->GetName().compare((*iter)->GetName()) == 0)
//		{
//			transferItem = *iter;
//			entity->bagObjects.erase(iter);
//			break;
//		}
//	}
//	if (transferItem != nullptr)
//	{
//		entity->equipedObjects.push_back(transferItem);
//		App->buff->AddItemStats(transferItem);
//	}
//}
//
//void j1ItemsManager::DesequipItem(j1Entity* entity,items * item)
//{
//	items* transferItem = nullptr;
//	std::vector<items*>::iterator iter = entity->equipedObjects.begin();
//	for (; iter != entity->equipedObjects.end(); ++iter)
//	{
//		if (item->GetName().compare((*iter)->GetName()) == 0)
//		{
//			transferItem = *iter;
//			App->buff->RemoveItemStat(*iter);
//			entity->equipedObjects.erase(iter);
//			break;
//		}
//	}
//	if(transferItem != nullptr)
//		entity->bagObjects.push_back(transferItem);
//}
//
//void j1ItemsManager::DesequipSimilarItem(j1Entity * entity, items * item)
//{
//	items* transferItem = nullptr;
//	std::vector<items*>::iterator iter = entity->equipedObjects.begin();
//	for (; iter != entity->equipedObjects.end(); ++iter)
//	{
//		if (item->GetType() == (*iter)->GetType())
//		{
//			transferItem = *iter;
//			entity->equipedObjects.erase(iter);
//			App->buff->RemoveItemStat(*iter);
//			break;
//		}
//	}
//	entity->bagObjects.push_back(transferItem);
//}
//
//void j1ItemsManager::LoadItemData(items* item)
//{
//
//
//	for (pugi::xml_node iterator = node.child("item"); iterator != nullptr; iterator = iterator.next_sibling("item"))
//	{
//		for (pugi::xml_node iterator2 = iterator.child("buff"); iterator2 != nullptr; iterator2 = iterator2.next_sibling("buff"))
//		{
//			if (item->GetName().compare(iterator.attribute("name").as_string()) == 0)
//			{
//				BUFF_TYPE type;
//				std::string data = iterator2.attribute("type").as_string();
//				if (data.compare("additive") == 0)
//					type = BUFF_TYPE::ADDITIVE;
//				else if (data.compare("multiplicative") == 0)
//					type = BUFF_TYPE::MULTIPLICATIVE;
//				else
//					type = BUFF_TYPE::NO_BUFF;
//
//				std::string character = iterator2.attribute("character").as_string();
//
//				ELEMENTAL_TYPE element;
//				data.assign(iterator2.attribute("element").as_string());
//				if (data.compare("poison") == 0)
//					element = ELEMENTAL_TYPE::POISON_ELEMENT;
//				else if (data.compare("fire") == 0)
//					element = ELEMENTAL_TYPE::FIRE_ELEMENT;
//				else if (data.compare("ice") == 0)
//					element = ELEMENTAL_TYPE::ICE_ELEMENT;
//				else
//					element = ELEMENTAL_TYPE::NORMAL_ELEMENT;
//
//				OBJECT_ROL rol;
//				data.assign(iterator2.attribute("rol").as_string());
//				if (data.compare("defence") == 0)
//					rol = OBJECT_ROL::DEFENCE_ROL;
//				else if (data.compare("attack") == 0)
//					rol = OBJECT_ROL::ATTACK_ROL;
//				else
//					rol = OBJECT_ROL::NO_ROL;
//
//				float value = iterator2.attribute("value").as_float();
//				
//				std::string stat = iterator2.attribute("stat").as_string();
//				item->CreateBuff(type, character,stat, element, rol, value);
//			}
//		}
//	}
//
//}
