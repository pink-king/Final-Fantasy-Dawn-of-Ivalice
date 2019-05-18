#ifndef __BOSSEMMITER_H__
#define __BOSSEMMITER_H__

#include "Projectile.h"
#include "j1Timer.h"
#include "j1Entity.h"
#include <list>
#include <random>

class BossEmmiter : public Projectile
{
public:
	BossEmmiter(fPoint pos, const j1Entity* owner, uint timeLife);
	~BossEmmiter();

	bool PreUpdate() override;
	bool Update(float dt) override;
	bool PostUpdate() override;

	void CreateArrow();
	float RandomValue(int min, int max);
private:
	bool deathTimerStarted = false;
	uint lifeTime;
	uint createArrowsSpeed;
	uint dieTime;
	j1Timer lifeTimer;
	j1Timer createArrowsTimer;
	j1Timer dieTimer;

	iPoint rang;
	int constantHeigth;
	std::random_device rd;
	std::mt19937 engine;

};


#endif  