#ifndef __MAGICBOLT_H__
#define __MAGICBOLT_H__

#include "Projectile.h"

class MagicBolt : public Projectile
{
public:
	MagicBolt(fPoint pos, fPoint destination, uint speed, const j1Entity* owner);
	~MagicBolt();

	bool PreUpdate() override;
	bool Update(float dt) override;
	bool Move(float dt) override;
	void Explode();

};


#endif 
