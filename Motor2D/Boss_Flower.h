#ifndef __FLOWER_BOSS_H__
#define __FLOWER_BOSS_H__

#include "j1Entity.h"
#include "PlayerEntity.h"

#define NUM_NEIGH_PATTERN 8

enum class Boss1State
{
	// phase2,3 and 4 increases patternCounter counter, decide what phase is the next "round" , 
	// phase 2 and 3 are switched until boss life decreases to 50% or less, instead always the loop is phase 1 - phase 4
	// boss loop:
	// - 50% to 100% life:
	//   - phase1 - phase2 - phase1 - phase 3
	// - 50% to 0% life:
	//   - phase1 - phase4
	// extra notes:
	// between 100% to 50% of life the evasion probability are fixed on X value
	// between 50% to 0% the evasion probability increases between x to around 50% chance

	NOTHING,
	PHASE1, // normal basic fireball pattern state
	PHASE2, // shield + poison rain
	PHASE3, // shield + spawn enemies
	PHASE4, // phase 2 + phase 3 (when the enemy life is 50% or less)
	PHASE5, // circle of death, if life <= 10% spawn replicas rotation around it with phase1 only
	//
	DEATH,
	MAX
};

struct BossPhaseTimer
{
	j1Timer timer;
	uint32 time;
};

struct BossPhaseTimers
{
	BossPhaseTimer phase1;
	BossPhaseTimer phase2;
	BossPhaseTimer phase3;
	BossPhaseTimer phase4;
};

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

	void LookToPlayer(Animation* desiredAnim);

	float lerpAngle(float a1, float a2, float t);
	void CheckRenderFlip();
	void PhaseManager(float dt);
	//void FollowPlayer(float dt);

	void ActiveShield();
	void DesactiveShield();
	void Phase2Logic(); // independent logic, phase 4 call this two
	void Phase3Logic();
	void DoShieldLogic();
	void ShieldLogic();
	bool IsAttackOnTilePerimeter();
	//void CheckFor

private:
	Boss1State myState = Boss1State::NOTHING;
	bool shieldActive = false;
	bool evading = false;
	SDL_Texture* boss_spritesheet = nullptr;
	// animations
	Animation idleAnim[(int)facingDirection::MAX];
	Animation attackAnim[(int)facingDirection::MAX];
	Animation jumpAnim[(int)facingDirection::MAX];
	Animation deathAnim;

	iPoint adjacentTileNeighboursPattern[NUM_NEIGH_PATTERN];

	int pointingDir = 0;
	float lastAngle;
	float currentAngle;

	//
	float dodgeProbability = 0.f; // dodge probability only affects on phase1, and increases probability between 50% to 0% of life
	int patternsCounter = 0; // decide what phase is the next

	// needed timers ----
	// phase timer switchers
	BossPhaseTimers phase_control_timers;
	BossPhaseTimer fireball_timer_data;
	bool doingAttack = false;
	BossPhaseTimer spawnEnemies_timer_data;
	BossPhaseTimer shieldFire_timer_data;
	//BossPhaseTimer poisonRain_timer_data;
	bool shootedPoisonRainEmitter = false;
	BossPhaseTimer maxEvasion_timer_data;

};

#endif // !__FLOWER_BOSS_H__
