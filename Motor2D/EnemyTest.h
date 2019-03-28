#ifndef __ENEMYTEST_H
#define __ENEMYTEST_H

#include "j1Entity.h"
#include "p2Animation.h"
#include "p2Point.h"
#include <vector>

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
	iPoint GetNextNode();
	bool SearchNewPath(); 
	void MoveToNextNode(float dt); 
	
	virtual bool CleanUp();

	void DebugPath() const;

	// functionality ------

	void Draw();

private:
	std::vector<iPoint> path_to_follow;

	Animation idle; 
	iPoint direction = { 0,0 };
	iPoint velocity = { 0,0 };
	uint chasingSpeed = 100;
};

#endif