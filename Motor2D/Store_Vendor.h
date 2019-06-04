#ifndef _STORE_VENDOR_H_
#define _STORE_VENDOR_H_


#include "LootEntity.h"

#define baseEquipables 10
#define baseCosumables 9
#define numberItemsLvlUp 10

class Vendor
{

public:

	std::vector<LootEntity*>		vBagObjects;
	std::vector<LootEntity*>		vConsumables;
	bool                            firstTime = true;

public:

	Vendor() {};
	~Vendor() {};


	void generateVendorItems(bool playerLvlUp = false);  // todo: every time the player levels up, a new unique item must appear 

	void EquipVendor(LootEntity* entityLoot, bool fromPlayer = false); 

	void DeEquipVendor(LootEntity* entityLoot); 

	void cleanUp(); 

public: 

	uint numberOfEquip; 
	uint numberOfPot; 


};
#endif 
