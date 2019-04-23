#ifndef __EARTHSHAKER_H__
#define __EARTHSHAKER_H__

#include "Projectile.h"
#include <random>

class EarthShaker : public Projectile
{
public:
	EarthShaker(fPoint pos, const j1Entity* owner);
	~EarthShaker();
	
	bool Update(float dt) override;
	bool PostUpdate() override; 

	void Attack(); 
	void SpawnTornados(); 

private: 
	uint tornados;
	std::random_device rd;  
	std::mt19937 gen;
};





#endif