#ifndef __ENEMYPROJECTILE_H__
#define __ENEMYPROJECTILE_H__

#include "Projectile.h"

class EnemyProjectile : public Projectile
{
public: 

	EnemyProjectile(fPoint pos, fPoint destination, uint speed, const j1Entity* owner);
	~EnemyProjectile();

	bool PreUpdate() override;
	bool Update(float dt) override;
	bool Move(float dt) override;
	bool Contact();
	void Draw() override; 
};


#endif