/*#ifndef _UIITEM_HITPOINTMANAGER_H_
#define _UIITEM_HITPOINTMANAGER_H_

#include <list>
#include "j1Module.h"
#include "UiItem_HitPoint.h"

class UiItem_HitPointManager : public j1Module
{
public:
	UiItem_HitPointManager();
	~UiItem_HitPointManager();

	bool Awake(pugi::xml_node&);
	bool Start();
	bool PreUpdate() { return true; };
	bool Update(float dt);
	bool PostUpdate() { return true; };
	bool CleanUp();

	void calculateHitPointSpawning(); // TODO: variables =  enemy pos, enemy rect top, enemy damage (buffmanager)   // TODO: call "AddHitPoint"
	void updateHitPointPositions();   // TODO: increment all current hitpoins Y position 
	void updateHitPointSizes();   // TODO: switch current hitpoint's states, and change size depending on the state    // use the scaling factor variable inside each hitbox
	void updateHitPointOpacities();   // TODO: switch current hitpoint's states, and change opacity depending on the state  (fadeIn and fadeOut)
	void DestroyHitPointLabel(); // TODO: unload texture, and nullptr, etc


private:
	std::list<UiItem_HitPoint*>			hitPointLabels;   // caution: there is already a ui item list, so hitpoint will appear in both lists ????!!!!

};

#endif*/
