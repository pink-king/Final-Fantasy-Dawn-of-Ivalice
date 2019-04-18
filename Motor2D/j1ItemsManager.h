//#ifndef _J1ITEMSMAMANAGER_H_
//#define _J1ITEMSMAMANAGER_H_
//
//#include "j1Module.h"
//#include "Items.h"
//#include "j1Entity.h"
//
//class j1ItemsManager : public j1Module
//{
//public:
//	j1ItemsManager();
//	~j1ItemsManager();
//
//	bool Awake(pugi::xml_node&);
//	bool Start();
//	bool PreUpdate() { return true; };
//	bool Update(float dt);
//	bool PostUpdate() { return true; };
//	bool CleanUp();
//	//take buffs to html
//	void CreateItem(std::string name, OBJECT_TYPE type, j1Entity* entity);
//	void RemoveItem(j1Entity* entity, std::string name);
//
//	void EquipItem(j1Entity* entity,items* item);
//	void DesequipItem(j1Entity* entity, items* item);
//	void DesequipSimilarItem(j1Entity* entity, items* item);
//
//	void LoadItemData(items* item);
//
//private:
//	pugi::xml_node		 node;
//};
//
//#endif
