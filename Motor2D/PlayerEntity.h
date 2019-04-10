#ifndef __PLAYERENTITY_H__
#define __PLAYERENTITY_H__

#include "p2Point.h"
#include "p2Animation.h"
#include "j1Entity.h"
#include "Buff.h"




#define PI 3.14159265359f

struct SDL_Texture;

enum class facingDirection // WARNING: no modify this order
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

enum class combatState
{
	IDLE,
	BASIC,
	DODGE,
	SPECIAL1,
	SPECIAL2,
	ULTIMATE,
	MAX
};

enum class characterName
{
	MARCHE,
	RITZ,
	SHARA
};

struct coolDown
{
	struct data
	{
		uint32 cooldownTime;
		j1Timer timer;
	};

	data basic,dodge,special1,special2,ultimate;
};

class PlayerEntity : public j1Entity
{
public:
	PlayerEntity(int posX, int posY);
	~PlayerEntity();

	//core loops ------
	//bool Awake(pugi::xml_node& node);
	bool Start();
	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();
	virtual bool CleanUp();

	// functionality ------
	bool InputMovement(float dt);
	bool InputCombat();
	int GetPointingDir(float angle);
	float GetLastHeadingAngle() const;
	void CheckRenderFlip(); // animation relative
	void Draw();

	bool IsAiming()
	{
		return aiming;
	}
	
public:

	bool aiming = false;
	
public:
	combatState combat_state;
	coolDown coolDownData;

public:
	
	float lastAxisMovAngle = 0.f;
	fPoint previousPos;
	int pointingDir = 0; // relative to facing direction enum order
	characterName character;
	int exp = 0; // TODO: BUFF MANAGER
	fPoint characterBaseSpeed = { 2.0f,1.5f }; // TODO: IMPLEMENT BUFF MANAGER and adds a percentage multiplicative with equipped items
	fPoint characterBaseSpeedKey = { 2.0f, 1.0f };
	SDL_Texture* spritesheet = nullptr;
	Animation idle[(int)facingDirection::MAX];
	Animation run[(int)facingDirection::MAX];
	Animation basicAttack[(int)facingDirection::MAX];
	Animation dodge[(int)facingDirection::MAX];
	Animation receiveDamage[(int)facingDirection::MAX];
	
	/*std::vector<j1Entity*>	entities;
	std::vector<j1Entity*> draw_entities;*/
	std::vector<Buff*>		buffs;
private:
	j1Timer inputDelayer;
	j1Timer pulsationTimeRecorder;

};

#endif