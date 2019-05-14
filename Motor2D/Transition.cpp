#include "Transition.h"
#include "j1TransitionManager.h"
#include "j1App.h"
#include "p2Log.h"

Transition::Transition(float transition_time)
{
	this->transition_time = transition_time;

	//start timer
	current_time = new j1Timer();
	current_time->Start();

	state = TransitionState::ENTERING;

	App->transitionManager->transitioning = true;
}


Transition::~Transition()
{
	delete current_time;
}

void Transition::PostUpdate()
{
	switch (state)
	{
	case Transition::TransitionState::NONE:
		break;
	case Transition::TransitionState::ENTERING:
		Entering();
		break;
	case Transition::TransitionState::ACTION:
		Action();
		break;
	case Transition::TransitionState::EXITING:
		Exiting();
		break;
	default:
		break;
	}
}

void Transition::Entering()
{
	percent = current_time->ReadSec()*(1 / transition_time);

	if (current_time->ReadSec() >= transition_time)
	{
		state = TransitionState::ACTION;
	}
}

void Transition::Action()
{
	current_time->Start();
	state = TransitionState::EXITING;
}

void Transition::Exiting()
{
	percent = current_time->ReadSec()*(1 / transition_time);

	if (current_time->ReadSec() >= transition_time)
	{
		state = TransitionState::NONE;
		App->transitionManager->transitioning = false;
		App->transitionManager->DestroyTransition(this);
	}
}

float Transition::LerpValue(float percent, float start, float end)
{
	return start + percent * (end - start);
}