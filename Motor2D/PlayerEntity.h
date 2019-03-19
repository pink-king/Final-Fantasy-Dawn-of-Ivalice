#ifndef __PLAYERENTITY_H__
#define __PLAYERENTITY_H__

#include "p2Point.h"
#include "p2Animation.h"
#include "j1Entity.h"

struct SDL_Texture;

enum class facingDirection
{
	N,
	S,
	E,
	W,
	NE,
	SE,
	NW,
	SW,
	MAX
};

enum class characterName
{
	MARCHE,
	RITZ,
	SHARA
};

class PlayerEntity : public j1Entity
{
public:
	PlayerEntity();
	~PlayerEntity();

	//core loops ------
	//bool Awake(pugi::xml_node& node);
	bool Start();
	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();
	//virtual bool CleanUp();

	// functionality ------
	bool InputMovement(float dt);
	

public:

	//j1Entity*				Player = nullptr;

public:

	characterName character;
	int exp = 0; // TODO: BUFF MANAGER
	fPoint characterBaseSpeed = { 2.0f,2.0f }; // TODO: IMPLEMENT BUFF MANAGER and adds a percentage multiplicative with equipped items
	SDL_Texture* spritesheet = nullptr;
	Animation idle[(int)facingDirection::MAX];
	Animation run[(int)facingDirection::MAX];
	Animation basicAttack[(int)facingDirection::MAX];
	Animation dodge[(int)facingDirection::MAX];
	Animation receiveDamage[(int)facingDirection::MAX];
	
	/*std::vector<j1Entity*>	entities;
	std::vector<j1Entity*> draw_entities;*/

};

#endif