#ifndef __EARTHSHAKER_H__
#define __EARTHSHAKER_H__

#include "Projectile.h"


class EarthShaker : public Projectile
{
public:
	EarthShaker(fPoint pos, const j1Entity* owner);
	~EarthShaker();
	
	bool Update(float dt) override;
	bool PostUpdate() override; 

	void SpawnTornado(); 


private: 
	j1Timer timer; 

};





#endif