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
	void LoadAnims();

private: 
	j1Timer checkTime; 
	bool freePass = false;
	bool attacked = false; 

};




#endif