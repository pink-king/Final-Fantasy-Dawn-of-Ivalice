#ifndef __MARCHE_H__
#define __MARCHE_H__

#include "PlayerEntity.h"

class Marche : public PlayerEntity
{
public:
	Marche();
	~Marche();

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