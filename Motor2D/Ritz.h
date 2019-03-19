#ifndef __RITZ_H__
#define __RITZ_H__

#include "PlayerEntity.h"

class Ritz : public PlayerEntity
{
public:
	Ritz();
	~Ritz();

	//core loops ------
	//bool Awake(pugi::xml_node& node);
	bool Start();
	bool PreUpdate();
	bool Update(float dt);
	//bool PostUpdate();
	//virtual bool CleanUp();

	// functionality ------


public:


private:

};

#endif