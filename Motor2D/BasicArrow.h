#ifndef __BASICARROW_H__
#define __BASICARROW_H__

#include "Projectile.h"
#include "p2Point.h"

class BasicArrow : public  Projectile
{
public:
	BasicArrow(fPoint pos, fPoint destination, uint speed, const j1Entity* owner);
	~BasicArrow();

	bool PreUpdate() override;
	bool Update(float dt) override;
	bool Move(float dt) override;
	bool Contact();

};

#endif