#ifndef __ENEMYDUMMY_H__
#define __ENEMYDUMMY_H__

#include "Enemy.h"


class EnemyDummy : public Enemy
{
public:
	EnemyDummy(const iPoint& pos); 
	~EnemyDummy();

	bool Update(float dt) override; 
	bool PostUpdate() override; 
	bool CleanUp() override; 

private: 
	float maxLife = 10000.F;
	Animation idle; 
};

#endif