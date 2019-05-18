#ifndef __ENEMYARCHER_H__
#define __ENEMYARCHER_H__

#include "Enemy.h"


class EnemyArcher : public Enemy
{
public: 

	EnemyArcher(const iPoint& pos, bool dummy = false); 
	~EnemyArcher(); 

	bool PreUpdate() override; 
	bool Update(float dt) override;
	bool PostUpdate() override;
	bool CleanUp() override;

	// functionality ------

	void SetState(float dt);
	bool isOnMeleeRange(); 
	void LoadAnims();

private: 

	Animation meleeAttack[(int)facingDirectionEnemy::MAX];

	j1Timer checkTime; 
	bool freePass = false;
	bool attacked = false; 
	bool toAttackMelee = false; 
};




#endif