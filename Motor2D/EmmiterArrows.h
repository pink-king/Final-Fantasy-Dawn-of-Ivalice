#ifndef __EMMITERARROWS_H__
#define __EMMITERARROWS_H__

#include "Projectile.h"
#include "p2Point.h"

class EmmiterArrows : public  Projectile
{
public:
	EmmiterArrows(fPoint pos, fPoint destination, uint speed, const j1Entity* owner, uint timeLife);
	~EmmiterArrows();

	bool PreUpdate() override;
	bool Update(float dt) override;
	bool Move(float dt) override;

	bool CleanUp() override;

	void Draw();
private:

	j1Timer timeLifeTimer;
	uint	timeLife;

	bool damage = true;
};

#endif