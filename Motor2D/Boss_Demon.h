#ifndef __DEMON_BOSS_H__
#define __DEMON_BOSS_H__

#include "j1Entity.h"
#include "PlayerEntity.h"

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

private:
	int GetPointingDir(float angle);

	void FollowPlayer(float dt);

private:
	SDL_Texture* boss_spritesheet = nullptr;
	// animations
	Animation walkCycleAnim[(int)facingDirection::MAX];
	Animation attackAnim[(int)facingDirection::MAX];
	Animation blockAnim[(int)facingDirection::MAX];
	Animation teleportAnim[(int)facingDirection::MAX];

	int pointingDir = 0;

};

#endif // !__DEMON_BOSS_H__
