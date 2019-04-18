#ifndef __ENTITYARROW_H__
#define __ENTITYARROW_H__

#include "j1Entity.h"
#include "p2Point.h"

#define DESTRUCTIONRANGE 550

class EntityArrow : public  j1Entity
{
public:
	EntityArrow(fPoint pos, fPoint destination, uint speed, const j1Entity* owner);
	~EntityArrow(); 
	
	bool PreUpdate() override; 
	bool Update(float dt) override;
	bool Move(float dt) override; 
	
	bool CheckMyPos();
	bool Contact(); 
	float SetMyAngleRotation(const fPoint& direction); 
	bool TooFarAway() const; 
	bool CollisionWithWall() const; // Some abilities may ignore walls (?)
	bool CleanUp() override; 
	void Draw() override; 

private:
	
	Animation flying;
	const j1Entity* owner = nullptr; 
	SDL_Texture* debugSubtile = nullptr;
	fPoint destination = { 0, 0 };
	fPoint direction = { 0,0 };
	uint speed = 0;
	float angle = 0.F; 
	bool to_explode = false; 
};

#endif