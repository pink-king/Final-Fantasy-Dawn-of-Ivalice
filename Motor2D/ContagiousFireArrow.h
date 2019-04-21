#ifndef __CONTAGIOUSFIREARROW_H__
#define __CONTAGIOUSFIREARROW_H__

#include "Projectile.h"
#include "p2Point.h"

#define DESTRUCTIONRANGE 550

class ContagiousFireArrow : public  Projectile
{
public:
	ContagiousFireArrow(fPoint pos, fPoint destination, uint speed, const j1Entity* owner);
	~ContagiousFireArrow();

	bool PreUpdate() override;
	bool Update(float dt) override;
	bool Move(float dt) override;
	bool Explode();
	void SpawnParticles(); 

private: 
	j1Timer timer; 
	bool hitwall = false; 
	bool wallImpact;

};

#endif