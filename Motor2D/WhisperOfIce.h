#ifndef __WHISPEROFICE_H__
#define __WHISPEROFICE_H__

#include "Projectile.h"

class WhisperOfIce : public Projectile
{
public: 
	WhisperOfIce(const fPoint& pos, const fPoint& destination, uint speed, uint lifeTime, const j1Entity* owner);
	~WhisperOfIce();

	bool PreUpdate() override;
	bool Update(float dt) override; 
	bool PostUpdate() override; 
	bool Move(float dt) override; 

private: 

	void Propagate();

private: 

	uint lifeTime = 0; 
	j1Timer timer; 

	uint propagationSpeed = 200; 
	j1Timer propagationTimer; 

	Animation startAnim; 
	Animation loopingAnim; 
	Animation endingAnim;
};

#endif // !__WHISPEROFICE_H__
