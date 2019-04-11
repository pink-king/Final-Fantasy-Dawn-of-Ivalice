#ifndef __MARCHE_H__
#define __MARCHE_H__

#include "PlayerEntity.h"
#include "GUI_Definitions.h"


struct myClocks; 

class Marche : public PlayerEntity
{
public:
	Marche(int posX, int posY);
	~Marche();

	//core loops ------
	//bool Awake(pugi::xml_node& node);
	bool Start();
	bool PreUpdate();
	bool Update(float dt);
	//bool PostUpdate();
	virtual bool CleanUp();

	// functionality ------


public:

    
	myClocks myUIClocks;

private:

};

#endif