#ifndef __CHESTASSET_H__
#define __CHESTASSET_H__

#include "j1Entity.h"


class ChestAsset : public j1Entity
{
public:

	ChestAsset(const iPoint& pos);
	~ChestAsset();

	//bool Start() override;	// Entity Factory doesn't Start the entities when reloading scenes!
	bool PreUpdate() override;

	void Draw() override;

	void OpenChest();

private:

	Animation idle;
	Animation opening;

	bool isAssigned = false;
	bool isOpened = false;
};



#endif