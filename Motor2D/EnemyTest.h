#ifndef __ENEMYTEST_H
#define __ENEMYTEST_H

#include "j1Entity.h"
#include "p2Animation.h"
#include "p2Point.h"

class EnemyTest : public j1Entity
{
public:
	EnemyTest(iPoint position);
	~EnemyTest();

	//core loops ------
	//bool Awake(pugi::xml_node& node);
	bool Start();
	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();
	virtual bool CleanUp();

	// functionality ------

	void Draw();

private:
	Animation idle; 
};

#endif