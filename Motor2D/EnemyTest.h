#ifndef __ENEMYTEST_H
#define __ENEMYTEST_H

#include "j1Entity.h"
#include "p2Animation.h"
#include "p2Point.h"
#include <vector>


#define RANGE 20

enum class EnemyState
{
	WAITING,
	IDLE, 
	SEARCHPATH,
	GET_NEXT_TILE,
	GO_NEXT_TILE,
	MAX
};

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
	void SetState(float dt);
	bool SearchNewPath(); 
	virtual bool CleanUp();

	void DebugPath() const;
	// functionality ------

	void Draw();

private:
	std::vector<iPoint> path_to_follow;

	iPoint tileToGo; 
	EnemyState state = EnemyState::IDLE; 
	Animation idle; 
	iPoint direction = { 0,0 };
	fPoint velocity = { 0,0 };
	uint chasingSpeed = 100;
};

#endif