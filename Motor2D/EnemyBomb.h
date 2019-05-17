#ifndef __ENEMYBOMB_H
#define __ENEMYBOMB_H

#include "Enemy.h"
#include "p2Animation.h"
#include "p2Point.h"
#include "j1Timer.h"
#include <vector>

struct SDL_Texture; 


class EnemyBomb : public Enemy
{
public: 

	EnemyBomb(iPoint position, uint speed, uint detectionRange, uint attackRange, uint baseDamage);
	EnemyBomb(iPoint position, bool dummy);
	~EnemyBomb();

	bool Start() override;
	bool PreUpdate() override;
	bool Update(float dt) override;
	bool PostUpdate() override;
	bool CleanUp() override;

	void LoadAnims();
	// functionality ------

	void SetState(float dt);

private:
	j1Timer checkTime;

	uint explosionRange = 0; 
	uint explosionDelay = 0;
	bool freePass = false;		// Needs a re-planing
	bool exploded = false; 
};


#endif