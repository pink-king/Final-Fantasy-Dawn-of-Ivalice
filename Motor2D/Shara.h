#ifndef __SHARA_H__
#define __SHARA_H__

#include "PlayerEntity.h"
#include "GUI_Definitions.h"

class Shara : public PlayerEntity
{
public:
	Shara(int posX,int posY);
	~Shara();

	//core loops ------
	//bool Awake(pugi::xml_node& node);
	bool Start();
	bool PreUpdate();
	bool Update(float dt);
	//bool PostUpdate();
	//virtual bool CleanUp();
	
	// functionality ------
	bool dodgedtest = false;

private:
	bool SetStopperState();
	void SpawnArrowsInArch(const fPoint& destination); 

public:
	myClocks myUIClocks;

private:
	fPoint dashPivotOffset[(int)facingDirection::MAX][4];
	fPoint ultiCastPivots[(int)facingDirection::MAX];
	Animation ultiCastAnim[(int)facingDirection::MAX];
	SDL_Texture* ultiCastTex = nullptr;
	// TODO
	j1Timer ultiEmitterTimerDelayer;
	Uint32 maxEmitterDelayTime;
	bool instantiateEmitter = false;
};

#endif