#ifndef __DEMON_BOSS_H__
#define __DEMON_BOSS_H__

#include "j1Entity.h"

class DemonBossEntity : public j1Entity
{
public:
	DemonBossEntity(iPoint position);
	~DemonBossEntity();

	// ---------------
	//bool Start();
	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();
	bool CleanUp();
	void Draw();

public:
	SDL_Texture* boss_spritesheet = nullptr;
	// animations
	Animation walkCycleAnim[8];
	Animation attackAnim[8];
	Animation blockAnim[8];
	Animation teleportAnim[8];


};

#endif // !__DEMON_BOSS_H__
