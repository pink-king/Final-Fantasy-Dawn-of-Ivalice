#ifndef __BREAKABLEASSET_H__
#define __BREAKABLEASSET_H__

#include "j1Entity.h"

enum class BreakableType; 

class BreakableAsset : public j1Entity
{
public:

	BreakableAsset(const iPoint& pos, BreakableType type, bool isBroken, bool containsLoot = true);
	~BreakableAsset();

	//bool Start() override;	// Entity Factory doesn't Start the entities when reloading scenes!
	bool PreUpdate() override; 
	bool Update(float dt) override;

	void Draw() override;

	bool Load(pugi::xml_node&) override;
	bool Save(pugi::xml_node&) const override;

	void Break();

private:

	Animation idle;
	Animation breaking;

	BreakableType breakableType;

	bool isAssigned = false;
	bool isBroken = false;
	bool PlaySFX = true;
	bool containsLoot = true; 
};



#endif