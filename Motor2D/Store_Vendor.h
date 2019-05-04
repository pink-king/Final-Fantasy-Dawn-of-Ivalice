#ifndef _STORE_VENDOR_H_
#define _STORE_VENDOR_H_


#include "LootEntity.h"


class Vendor
{

public:

	std::vector<LootEntity*>		vBagObjects;
	std::vector<LootEntity*>		vConsumables;
	bool                            firstTime = true;

public:

	Vendor() {};
	~Vendor() {};


	void generateVendorItems();  // todo: every time the player levels up, a new unique item must appear 

	void EquipVendor(LootEntity* entityLoot); 

	void DeEquipVendor(LootEntity* entityLoot); 

	void cleanUp(); 




};
#endif 
