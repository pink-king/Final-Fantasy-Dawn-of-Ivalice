#ifndef __RITZ_H__
#define __RITZ_H__

#include "PlayerEntity.h"
#include "GUI_Definitions.h"
#include "Projectile.h"

class Ritz : public PlayerEntity
{
public:
	Ritz(int posX, int posY);
	~Ritz();

	//core loops ------
	//bool Awake(pugi::xml_node& node);
	bool Start();
	bool PreUpdate();
	bool Update(float dt);

	//bool PostUpdate();
	//virtual bool CleanUp();

	// functionality ------
	bool CheckMyLastProjectile();

private:
	bool SetStopperState();
	fPoint GetTeleportPos();
	void ControlMyOwnProjectile();

	bool dodgedtest=false; // wtf is this

	// clocks
	myClocks myUIClocks;

private:

	fPoint dashPivotOffset[(int)facingDirection::MAX][4];
	fPoint attackPivotOffset[(int)facingDirection::MAX];
	Animation attack1[(int)facingDirection::MAX];
	Animation tpAnim[(int)facingDirection::MAX];
	SDL_Texture* attack1Tex = nullptr;
	SDL_Texture* teleportTex = nullptr;

	float tpMaxDistance;

	// testing for new possible incoming attack, whispers of ice 
	// (similar to tornado but controllable last launched attack and freeze all on its own way)
	// this have a limit of affected enemy units (for evade op), but if it touches a wall its destroy
	Projectile* lastProjectile = nullptr;

};

#endif