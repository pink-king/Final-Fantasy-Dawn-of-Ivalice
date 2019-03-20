#ifndef __SHARA_H__
#define __SHARA_H__

#include "PlayerEntity.h"

class Shara : public PlayerEntity
{
public:
	Shara();
	~Shara();

	//core loops ------
	//bool Awake(pugi::xml_node& node);
	bool Start();
	bool PreUpdate();
	bool Update(float dt);
	//bool PostUpdate();
	virtual bool CleanUp();

	// functionality ------


public:


private:

};

#endif