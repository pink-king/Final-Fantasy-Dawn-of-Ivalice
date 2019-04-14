#ifndef __ENEMYTEST_H
#define __ENEMYTEST_H

#include "Enemy.h"
#include "p2Animation.h"
#include "p2Point.h"
#include "j1Timer.h"
#include <vector>


struct SDL_Texture; 

//enum class EnemyState
//{
//	WAITING,
//	IDLE, 
//	SEARCHPATH,
//	GET_NEXT_TILE,
//	GO_NEXT_TILE,
//	CHECK,
//	ATTACK,
//	MAX
//};

class EnemyTest : public Enemy
{
public:
	EnemyTest(iPoint position, uint speed, uint detectionRange, uint attackRange, uint baseDamage, float attackSpeed);
	~EnemyTest();

	//core loops ------
	//bool Awake(pugi::xml_node& node);
	bool Start() override;
	bool PreUpdate() override;
	bool Update(float dt) override;
	bool PostUpdate() override;
	bool CleanUp() override;

	// functionality ------

	void SetState(float dt);
	bool CheckDistance();


private:
	j1Timer checkTime; 

	bool freePass = false;		// Needs a re-planing
};

#endif