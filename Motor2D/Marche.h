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
	/*void MarcheUltimate();*/
	bool PostUpdate();
	//virtual bool CleanUp();

	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&) const;
	// functionality ------


public:
	bool dodgedTest;
    
	myClocks myUIClocks;

private:
	// BASE DAMAGE
	int baseDamage;
	// BASIC ATTACK
	SDL_Texture* basicAttackTex = nullptr;
	Animation basicAttackAnim[(int)facingDirection::MAX][2];
	j1Timer basicAttackPulsationTimer;
	Uint32 basicAttackPulsationMaxTime;

	// ULTIMATE -------------------------
	SDL_Texture* superTransTex = nullptr;
	SDL_Texture* superRunTex = nullptr;
	SDL_Texture* superAttackTex = nullptr;
	Animation superTransAnim[(int)facingDirection::MAX];
	// i must to name this like this
	bool superSaiyajin = false;
	j1Timer superSaiyajinTimer;
	Uint32 superTransMaxTimeSec;
	fPoint superAnimPivots[(int)facingDirection::MAX];
	SDL_Texture* runTexTempPtr = nullptr;
	SDL_Texture* basicAttackTexPtr = nullptr;
	// -----------------------------------
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
	float maxTornadoDistanceMultiplier;

	// DASH pivoting
	fPoint dashPivotOffset[(int)facingDirection::MAX][4];
	fPoint basicAttackPivotOffset[(int)facingDirection::MAX];
};

#endif