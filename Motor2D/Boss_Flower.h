#ifndef __FLOWER_BOSS_H__
#define __FLOWER_BOSS_H__

#include "j1Entity.h"
#include "PlayerEntity.h"

class FlowerBossEntity : public j1Entity
{
public:
	FlowerBossEntity(iPoint position);
	~FlowerBossEntity();

	// ---------------
	//bool Start();
	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();
	bool CleanUp();
	void Draw();

private:
	int GetPointingDir(float angle);

	void LookToPlayer(float dt);

	float lerpAngle(float a1, float a2, float t);
	void CheckRenderFlip();
	void UpdatesCurrentAnimFrame();
	//void FollowPlayer(float dt);

private:
	SDL_Texture* boss_spritesheet = nullptr;
	// animations
	Animation idleAnim[(int)facingDirection::MAX];
	Animation attackAnim[(int)facingDirection::MAX];
	Animation jumpAnim[(int)facingDirection::MAX];
	Animation deathAnim[2];

	int pointingDir = 0;
	float lastAngle;
	float currentAngle;

};
#endif // !__FLOWER_BOSS_H__
