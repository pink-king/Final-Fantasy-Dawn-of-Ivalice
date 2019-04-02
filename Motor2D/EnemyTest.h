#ifndef __ENEMYTEST_H
#define __ENEMYTEST_H

#include "j1Entity.h"
#include "p2Animation.h"
#include "p2Point.h"
#include "j1Timer.h"
#include <vector>


#define RANGE 20
struct SDL_Texture; 

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
	bool isNextSubtileFree(int x, int y) const; 
	int GetRandomValue(const int& min, const int& max); 
	virtual bool CleanUp();

	void DebugPath() const;
	// functionality ------

	void Draw();

private:
	std::vector<iPoint> path_to_follow;
	
	EnemyState state = EnemyState::IDLE;

	iPoint tileToGo; 
	fPoint newPosition; 
	iPoint direction = { 0,0 };
	fPoint velocity = { 0,0 };
	uint speed = 100;

	Animation idle;
	j1Timer checkTime; 

	SDL_Texture* debugSubtile = nullptr; 
};

#endif