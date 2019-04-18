#ifndef __PROJECTILE_H__
#define __PROJECTILE_H__

#include "j1Entity.h"
#include "p2Point.h"

#define DESTRUCTIONRANGE 550

class Projectile : public j1Entity
{
public:
	Projectile(fPoint pos, fPoint destination, uint speed, const j1Entity* owner, const char* name);
	~Projectile(); 

	bool PostUpdate() override;		// Checks if its too far from the player and deletes it
	void SetNewDirection(const fPoint& newdir);		
	bool OnCollisionWithEnemy();	
	float SetMyAngleRotation(const fPoint& direction);	// Useful for render rotations
	bool TooFarAway() const;
	bool OnCollisionWithWall() const;
	bool ReachedDestiny() const; // Needs to take a look to the initial direction (vertical cases)
	void Draw() override;

protected: 

	Animation anim; 
	const j1Entity* owner = nullptr;
	SDL_Texture* debugSubtile = nullptr;
	fPoint destination = { 0, 0 };
	fPoint direction = { 0,0 };
	uint speed = 0;
	float angle = 0.F;
	bool to_explode = false;
};

#endif
