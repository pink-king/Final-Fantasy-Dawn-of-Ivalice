#ifndef __FIREARROW_H__
#define __FIREARROW_H__

#include "Projectile.h"

class FireArrow : public Projectile
{
public: 
	FireArrow(fPoint pos, fPoint destination, uint speed, const j1Entity* owner); 
	~FireArrow();

	bool CleanUp() override;
	bool PreUpdate() override;
	bool Update(float dt) override; 
	bool Move(float dt) override;
	void Pierce();
	void Explode(); 


private: 

	uint piercedEnemies = 0;
	uint maxPiercedEnemies = 3;
	bool attacked = false; 
};


#endif 
