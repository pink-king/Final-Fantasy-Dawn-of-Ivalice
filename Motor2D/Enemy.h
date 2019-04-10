#ifndef __ENEMY_H
#define __ENEMY_H

#include "j1Entity.h"
#include "p2Animation.h"
#include "p2Point.h"
#include "j1Timer.h"
#include <vector>


#define RANGE 20
struct SDL_Texture;
enum class facingDirectionEnemy // WARNING: no modify this order
{
	E, // right - 0
	SE, // bottom-right - 1
	S, // bottom - 2
	SW, // bottom-left - 3
	W, // left - 4
	NE, // upper-right - 5
	N, // up - 6
	NW, // upper-left - 7
	MAX
};

enum class EnemyState
{
	WAITING,
	IDLE,
	SEARCHPATH,
	GET_NEXT_TILE,
	GO_NEXT_TILE,
	CHECK,
	ATTACK,
	MAX
};

enum class EnemyType
{
	MELEE,
	DISTANCE,
	TRAP,
	TEST,
};

class Enemy : public j1Entity
{
public:
	Enemy(iPoint position, uint movementSpeed, uint detectionRange, uint attackRange);
	~Enemy();

	//core loops ------

	bool SearchNewPath();
	bool SearchNewSubPath();
	int GetRandomValue(const int& min, const int& max) const;
	bool isInDetectionRange() const;
	bool isInAttackRange() const;
	bool isNextPosFree(iPoint futurePos); 
	bool isOnDestiny() const; 
	void Draw() override;

	void DebugPath() const;
	// functionality ------


protected:
	std::vector<iPoint> path_to_follow;

	EnemyState state = EnemyState::IDLE;

	iPoint currentDestiny;
	iPoint direction = { 0,0 };
	fPoint velocity = { 0,0 };
	uint speed = 0;

	Animation idle[(int)facingDirectionEnemy::MAX];
	Animation run[(int)facingDirectionEnemy::MAX];
	Animation basicAttack[(int)facingDirectionEnemy::MAX];
	Animation receiveDamage[(int)facingDirectionEnemy::MAX];
	
	uint detectionRange = 0; 
	uint attackRange = 0; 
	bool isSubpathRange = false;

	SDL_Texture* debugSubtile = nullptr;
};

#endif