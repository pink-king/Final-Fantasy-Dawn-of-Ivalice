#ifndef __TORNADO_H__
#define __TORNADO_H__

#include "Projectile.h"
#include "p2Point.h"

class Tornado : public  Projectile
{
public:
	Tornado(fPoint pos, fPoint destination, uint speed, const j1Entity* owner);
	~Tornado();

	bool PreUpdate() override;
	bool Update(float dt) override;
	bool Move(float dt) override;
	bool Attack();

private:
	j1Timer AttackTimer;
	j1Timer LifeTimer;

	uint lifeTime;
	uint AttackTime;

	Animation finishAnim;
	Animation loopAnim;

	bool finishTornado;
	bool isMoving;
};

#endif