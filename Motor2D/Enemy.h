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

class Enemy : public j1Entity
{
public:
	Enemy(iPoint position);
	~Enemy();

	//core loops ------
	//bool Awake(pugi::xml_node& node);

	bool SearchNewPath();
	bool SearchNewSubPath();
	int GetRandomValue(const int& min, const int& max);
	void Draw() override;

	void DebugPath() const;
	// functionality ------


protected:
	std::vector<iPoint> path_to_follow;

	EnemyState state = EnemyState::IDLE;

	iPoint currentDestiny;
	iPoint direction = { 0,0 };
	fPoint velocity = { 0,0 };
	uint speed = 75;

	Animation idle[(int)facingDirectionEnemy::MAX];
	Animation run[(int)facingDirectionEnemy::MAX];
	Animation basicAttack[(int)facingDirectionEnemy::MAX];
	Animation receiveDamage[(int)facingDirectionEnemy::MAX];
	
	bool isSubpathRange = false;

	SDL_Texture* debugSubtile = nullptr;
};

#endif