#ifndef __BOSSEMMITERARROW_H__
#define __BOSSEMMITERARROW_H__

#include "Projectile.h"
#include "p2Point.h"

class BossEmmiterArrow : public  Projectile
{
public:
	BossEmmiterArrow(fPoint pos, fPoint destination, uint speed, const j1Entity* owner, uint timeLife);
	~BossEmmiterArrow();

	bool PreUpdate() override;
	bool Update(float dt) override;
	bool Move(float dt) override;

	bool CleanUp() override;

private:

	j1Timer timeLifeTimer;
	uint	timeLife;

	bool damage = true;
};

#endif 