#ifndef _TRANSITION_H_
#define _TRANSITION_H_

#include "j1Timer.h"
#include "p2Point.h"
#include "SDL\include\SDL_rect.h"
#include "Color.h"

class Transition
{
protected:
	enum class TransitionState {
		NONE,
		ENTERING, //Before the scene change
		ACTION, //The frame where the scene changes
		EXITING //After the Scene change
	};

protected:
	TransitionState state;

	float transition_time; // total transition entering and exiting time
	j1Timer* current_time = nullptr; 

	float percent = 0; //percent of the current respect the total time. It goes from 0 to 1

public:

	Transition(float transition_time);
	~Transition();

	void PostUpdate();

	//-----Update process-------

	virtual void Entering();
	virtual void Action();
	virtual void Exiting();

	float LerpValue(float percent, float start, float end);
	iPoint Lerp(float percent, iPoint origin, iPoint destination);

	//---Specific transitions methods-------
};

#endif // _TRANSITION_H_