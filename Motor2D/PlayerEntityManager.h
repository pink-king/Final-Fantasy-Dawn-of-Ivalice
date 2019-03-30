#ifndef __PLAYERENTITYMANAGER_H__
#define __PLAYERENTITYMANAGER_H__

#include <vector>
#include "j1Module.h"
//#include "j1EntityFactory.h"
#include "Marche.h"
#include "Ritz.h"
#include "Shara.h"
#include "j1Input.h"

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
	void Draw();

public:


private:

	characterName selectedCharacterName;
	PlayerEntity* selectedCharacterEntity = nullptr;

	Marche* marche = nullptr;
	Ritz* ritz = nullptr;
	Shara* shara = nullptr;

	std::vector<PlayerEntity*> characters;
	
	SDL_Texture*			texture = nullptr;
	SDL_Texture* debugTileTex = nullptr;
	SDL_Texture* debugSubtileTex = nullptr;
	bool debug = true;
	/*std::vector<j1Entity*>	entities;
	std::vector<j1Entity*> draw_entities;*/
	

};

#endif