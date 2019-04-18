#ifndef __ENTITYARROW_H__
#define __ENTITYARROW_H__

#include "Projectile.h"
#include "p2Point.h"

#define DESTRUCTIONRANGE 550

class EntityArrow : public  Projectile
{
public:
	EntityArrow(fPoint pos, fPoint destination, uint speed, const j1Entity* owner);
	~EntityArrow(); 
	
	bool PreUpdate() override; 
	bool Update(float dt) override;
	bool Move(float dt) override;
	bool Explode(); 

	bool CleanUp() override; 
	void Draw() override; 
};

#endif