#ifndef __BREAKABLEASSET_H__
#define __BREAKABLEASSET_H__

#include "j1Entity.h"

enum class BreakableType; 

class BreakableAsset : public j1Entity
{
public:

	BreakableAsset(const iPoint& pos, BreakableType type);
	~BreakableAsset();

	//bool Start() override;	// Entity Factory doesn't Start the entities when reloading scenes!
	bool PreUpdate() override; 
	bool Update(float dt) override;
	bool CleanUp() override;

	virtual void Draw() override;

	void Break();

private:

	Animation idle;
	Animation breaking;

	bool isAssigned = false;
	bool isBroken = false;
};



#endif