#ifndef __SHARA_H__
#define __SHARA_H__

#include "PlayerEntity.h"
#include "GUI_Definitions.h"

class Shara : public PlayerEntity
{
public:
	Shara(int posX,int posY);
	~Shara();

	//core loops ------
	//bool Awake(pugi::xml_node& node);
	bool Start();
	bool PreUpdate();
	bool Update(float dt);
	//bool PostUpdate();
	//virtual bool CleanUp();

	// functionality ------
	bool dodgedtest = false;

public:


private:
	fPoint dashPivotOffset[(int)facingDirection::MAX][4];

};

#endif