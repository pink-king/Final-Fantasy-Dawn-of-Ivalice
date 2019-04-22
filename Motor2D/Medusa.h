#ifndef __MEDUSA_H__
#define __MEDUSA_H__

#include "Projectile.h"


class Medusa : public Projectile 
{
public: 
	Medusa(fPoint pos, const j1Entity* owner);
	~Medusa(); 

	bool Update(float dt) override;
	bool PostUpdate() override;
	void Pulsation(); 
	void Explode(); 
	void CenterPulsation(); 

private: 

	uint maxRange; 
	uint pulsations = 0; 
	j1Timer timer; 
	j1Timer centerTimer; 
	int radius; 
	float propSpeed; 

};

#endif