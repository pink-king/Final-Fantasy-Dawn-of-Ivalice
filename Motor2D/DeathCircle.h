#ifndef __DEATHCIRCLE_H__
#define __DEATHCIRCLE_H__

#include "Projectile.h"
#include "j1Timer.h"

class DeathCircle : public Projectile
{
public:
	DeathCircle(fPoint pos, const j1Entity* owner);
	~DeathCircle(); 

	bool PreUpdate() override;
	bool Update(float dt) override;
	bool PostUpdate() override; 

	void Propagate() const; 

private:
	bool madeFinisher = false;
	bool deathTimerStarted = false; 
	uint lifeTime; 
	uint propagationSpeed;
	j1Timer lifeTimer; 
	j1Timer propagateTimer; 
};


#endif 