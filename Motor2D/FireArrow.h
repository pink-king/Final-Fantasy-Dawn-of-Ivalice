#ifndef __FIREARROW_H__
#define __FIREARROW_H__

#include "Projectile.h"

class FireArrow : public Projectile
{
public: 
	FireArrow(fPoint pos, fPoint destination, uint speed, const j1Entity* owner); 
	~FireArrow();

	bool PreUpdate() override;
	bool Update(float dt) override; 
	bool Move(float dt) override;
	void Pierce();
	void Explode(); 
	void SpawnTrail();

private: 

	fPoint lastPos;
	uint piercedEnemies = 0;
	uint maxPiercedEnemies = 3;
	bool attacked = false; 
	bool hitwall = false;
	j1Timer timer;
};


#endif 
