#ifndef __ENTITYARROW_H__
#define __ENTITYARROW_H__

#include "j1Entity.h"
#include "p2Point.h"


class EntityArrow : public  j1Entity
{
public:
	EntityArrow(fPoint pos, fPoint destination, uint speed);
	~EntityArrow(); 
	
	bool PreUpdate() override; 
	bool Update(float dt) override;
	bool Move(float dt) override; 
	bool CheckMyPos();
	bool Contact(); 
	bool CleanUp() override; 
	void Draw() override; 

private:
	
	SDL_Texture* debugSubtile = nullptr;
	fPoint destination = { 0, 0 };
	fPoint direction = { 0,0 };
	uint speed = 0;
	bool to_explode = false; 
};

#endif