#ifndef __MARCHE_H__
#define __MARCHE_H__

#include "PlayerEntity.h"
#include "GUI_Definitions.h"


struct myClocks; 

class Marche : public PlayerEntity
{
public:
	Marche(int posX, int posY);
	~Marche();

	//core loops ------
	//bool Awake(pugi::xml_node& node);
	bool Start();
	bool PreUpdate();
	bool Update(float dt);
	void UlitMarche();
	bool PostUpdate();
	//virtual bool CleanUp();

	// functionality ------


public:
	bool dodgedTest;
    
	myClocks myUIClocks;

private:
	// WHIRLWIND
	SDL_Texture* whirlwindTex = nullptr;
	SDL_Texture* whirlwindFireTex = nullptr;
	Animation whirlwindAnim[(int)facingDirection::MAX];
	Animation whirlwindFireAnim;
	Animation whirlwindFireAnimExitLoop;
	j1Timer whirlwindTotalCastingTimer;
	j1Timer whirlwindAttackTimer;
	Uint32 whirlwindMaxCastingTime;
	Uint32 whirlwindCadence;
	bool doingWhirlwind = false;
	fPoint whirlwindFirePos;
	Uint16 whirlwindFinalBoomDMG;
	Uint16 whirlwindWhileLoopDMG;

	// DASH pivoting
	fPoint dashPivotOffset[(int)facingDirection::MAX][4];
	
};

#endif