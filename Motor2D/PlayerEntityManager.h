#ifndef __PLAYERENTITYMANAGER_H__
#define __PLAYERENTITYMANAGER_H__

#include <vector>
#include "j1Module.h"
//#include "j1EntityFactory.h"
#include "Marche.h"
#include "Ritz.h"
#include "Shara.h"
#include "j1Input.h"

class Crosshair
{
public:
	Crosshair();
	~Crosshair();

	bool Start();
	bool Update(float dt);
	bool PostUpdate();
	bool Reset();
	bool CleanUp();

public:
	iPoint GetSubtilePoint();

private:
	bool ManageInput(float dt);
	iPoint GetPivotPos();
	j1Entity* SearchForTargetOnThisSubtile(const iPoint subtile) const;
	fPoint GetHeadingVector(float angle);

public:
	bool isReseted = false;
	fPoint relativePosition;
	fPoint position;
	iPoint pivotOffset; // the point of the "center" of the crosshair graphic
	SDL_Texture* tex = nullptr;
	Animation startAnim;
	Animation loopAnim;
private:
	j1Entity* clampedEntity = nullptr;
	fPoint sensitivitySpeed = { 5.f,5.f };
	bool clamped = false; // following a enemy entity
	float maxRadiusDistance;
};

class PlayerEntityManager : public j1Entity
{
public:

	PlayerEntityManager(iPoint position);
	~PlayerEntityManager();

	//core loops ------
	//bool Awake(pugi::xml_node& node);
	bool Start();
	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();
	bool CleanUp();

	//bool Draw();

	// functionality ------
	bool SwapInputChecker();
	void UpdatePivot(); // updates "manager" pivot to selected player(character) pivot
	//bool InputMovement();
	void SetPreviousCharacter();
	void SetNextCharacter();
	void SetCurrentAnimation();
	iPoint GetCrossHairSubtile();
	void Draw();

public:
	const j1Entity* GetSelectedCharacterEntity() const;
	const float GetLastPlayerHeadingAngle() const;

private:
	float lastCharHeadingAngle; // rad
	characterName selectedCharacterName;
	PlayerEntity* selectedCharacterEntity = nullptr;

	Marche* marche = nullptr;
	Ritz* ritz = nullptr;
	Shara* shara = nullptr;
	Crosshair* crossHair = nullptr;

	std::vector<PlayerEntity*> characters;
	
	SDL_Texture*			texture = nullptr;
	SDL_Texture* debugTileTex = nullptr;
	SDL_Texture* debugSubtileTex = nullptr;
	bool debug = true;
	/*std::vector<j1Entity*>	entities;
	std::vector<j1Entity*> draw_entities;*/
	

};

#endif