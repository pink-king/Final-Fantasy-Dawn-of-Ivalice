#include "j1EasingSplines.h"
#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include <string>
#include "p2Log.h"

#include "Brofiler/Brofiler.h"

j1EasingSplines::j1EasingSplines() : j1Module()
{
	name.assign("easingsplines");

}

// Destructor
j1EasingSplines::~j1EasingSplines()
{

}

// Called each loop iteration
bool j1EasingSplines::Update(float dt)
{
	BROFILER_CATEGORY("UpdateEasingSplines", Profiler::Color::Purple);

	std::list<EaseSplineInfo*>::iterator item = easing_splines.begin();

	for (; item != easing_splines.end(); ++item) {
		if (*item != nullptr) {
			if (!(*item)->Update(dt)) {
				delete(*item);
				(*item) = nullptr;
			}
		}
	}

	easing_splines.remove(nullptr);

	return true;
}

// Called before quitting
bool j1EasingSplines::CleanUp()
{
	LOG("Freeing scene");

	std::list<EaseSplineInfo*>::iterator item = easing_splines.begin();

	for (; item != easing_splines.end(); ++item) {
		if (*item != nullptr) {
			delete(*item);
			(*item) = nullptr;
		}
	}

	easing_splines.clear();

	return true;
}

EaseSplineInfo* j1EasingSplines::CreateSpline(float* position, const int target_position, const float time_to_travel, TypeSpline type, std::function<void()> fn)
{
	std::list <EaseSplineInfo*>::iterator item = easing_splines.begin();
	for (; item != easing_splines.end(); ++item) {
		if ((*item) != nullptr && (*item)->position == position) {
			(*item)->to_delete = true;
			break;
		}
	}

	EaseSplineInfo* info = DBG_NEW EaseSplineInfo(position, target_position, time_to_travel, type, fn);

	if (info != nullptr)
		easing_splines.push_back(info);
	else
		LOG("Could not create the Spline...");

	return info;
}




bool EaseSplineInfo::Update(float dt)
{
	bool ret = true;

	float time_passed = SDL_GetTicks() - time_started;

	if (time_passed < time_to_travel) {
		switch (type) {
		case EASE: {
			*position = ease_function.Ease(time_passed, initial_position, distance_to_travel, time_to_travel);
			LOG("Ease positionX %i", &position);
		} break;
		case EASE_OUT_QUINT: {
			*position = ease_function.EaseOutQuint(time_passed, initial_position, distance_to_travel, time_to_travel);
		} break;
		case EASE_IN_OUT_BACK: {
			*position = ease_function.EaseInOutBack(time_passed, initial_position, distance_to_travel, time_to_travel);
		} break;
		case EASE_IN_BACK: {
			*position = ease_function.EaseInBack(time_passed, initial_position, distance_to_travel, time_to_travel);
		} break;
		case EASE_OUT_BACK: {
			*position = ease_function.EaseOutBack(time_passed, initial_position, distance_to_travel, time_to_travel);
		} break;
		case EASE_IN_CUBIC: {
			*position = ease_function.EaseInCubic(time_passed, initial_position, distance_to_travel, time_to_travel);
		} break;
		case EASE_OUT_CUBIC: {
			*position = ease_function.EaseOutCubic(time_passed, initial_position, distance_to_travel, time_to_travel);
		} break;
		case EASE_OUT_BOUNCE: {
			*position = ease_function.EaseOutBounce(time_passed, initial_position, distance_to_travel, time_to_travel);
			LOG("Ease positionY %i", &position);

		} break;
		default:
			break;
		}
	}
	else {
		if (fn != nullptr)
			this->fn();
		to_delete = true;
		ret = false;
	}


	return ret;
}


int EaseFunctions::EaseOutQuint(float time_passed, int initial_position, int distance_to_travel, float time_to_travel)
{
	return distance_to_travel * ((time_passed = time_passed / time_to_travel - 1) * time_passed * time_passed * time_passed * time_passed + 1) + initial_position;
}

int EaseFunctions::Ease(float time_passed, int initial_position, int distance_to_travel, float time_to_travel)
{
	return distance_to_travel * (time_passed / time_to_travel) + initial_position;
}

int EaseFunctions::EaseInOutBack(float time_passed, int initial_position, int distance_to_travel, float time_to_travel)
{
	float s = 1.70158f;
	if ((time_passed /= time_to_travel / 2) < 1) {
		return distance_to_travel / 2 * (time_passed * time_passed * (((s *= (1.525f)) + 1) * time_passed - s)) + initial_position;
	}
	else {
		float postFix = time_passed -= 2;
		return distance_to_travel / 2 * ((postFix)* time_passed * (((s *= (1.525f)) + 1) * time_passed + s) + 2) + initial_position;
	}
}

int EaseFunctions::EaseInBack(float time_passed, int initial_position, int distance_to_travel, float time_to_travel)
{
	float s = 2.70158f;
	float postFix = time_passed /= time_to_travel;
	return distance_to_travel * (postFix)* time_passed* ((s + 1) * time_passed - s) + initial_position;
}

int EaseFunctions::EaseOutBack(float time_passed, int initial_position, int distance_to_travel, float time_to_travel)
{
	float s = 1.10158f;
	return distance_to_travel * ((time_passed = time_passed / time_to_travel - 1) * time_passed * ((s + 1) * time_passed + s) + 1) + initial_position;
}

int EaseFunctions::EaseInCubic(float time_passed, int initial_position, int distance_to_travel, float time_to_travel)
{
	return distance_to_travel * (time_passed /= time_to_travel) * time_passed * time_passed + initial_position;
}

int EaseFunctions::EaseOutCubic(float time_passed, int initial_position, int distance_to_travel, float time_to_travel)
{
	return distance_to_travel * ((time_passed = time_passed / time_to_travel - 1) * time_passed * time_passed + 1) + initial_position;
}

int EaseFunctions::EaseOutBounce(float time_passed, int initial_position, int distance_to_travel, float time_to_travel)
{
	if ((time_passed /= time_to_travel) < (1 / 2.75f)) {
		return distance_to_travel * (7.5625f * time_passed * time_passed) + initial_position;
	}
	else if (time_passed < (2 / 2.75f)) {
		float postFix = time_passed -= (1.5f / 2.75f);
		return distance_to_travel * (7.5625f * (postFix)* time_passed + .75f) + initial_position;
	}
	else if (time_passed < (2.5 / 2.75)) {
		float postFix = time_passed -= (2.25f / 2.75f);
		return distance_to_travel * (7.5625f * (postFix)* time_passed + .9375f) + initial_position;
	}
	else {
		float postFix = time_passed -= (2.625f / 2.75f);
		return distance_to_travel * (7.5625f * (postFix)* time_passed + .984375f) + initial_position;
	}
}