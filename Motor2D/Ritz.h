#ifndef __RITZ_H__
#define __RITZ_H__

#include "PlayerEntity.h"
#include "GUI_Definitions.h"

class Ritz : public PlayerEntity
{
public:
	Ritz(int posX, int posY);
	~Ritz();

	//core loops ------
	//bool Awake(pugi::xml_node& node);
	bool Start();
	bool PreUpdate();
	bool Update(float dt);
	//bool PostUpdate();
	//virtual bool CleanUp();

	// functionality ------

	bool dodgedtest=false;
public:
	myClocks myUIClocks;

private:
	fPoint dashPivotOffset[(int)facingDirection::MAX][4];
	Animation attack1[(int)facingDirection::MAX];
	SDL_Texture* attack1Tex = nullptr;

};

#endif