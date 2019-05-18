#ifndef __GOLEMPROJECTILE_H__
#define __GOLEMPROJECTILE_H__

#include "Projectile.h"

class GolemProjectile : public Projectile
{
public:

	GolemProjectile(fPoint pos, fPoint destination, uint speed, const j1Entity* owner);
	~GolemProjectile();

	bool PreUpdate() override;
	bool Update(float dt) override;
	bool Move(float dt) override;
	bool Contact();
};


#endif