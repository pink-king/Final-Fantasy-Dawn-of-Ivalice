#include "j1EasingSplines.h"
#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include <string>
#include "p2Log.h"
#include "j1Scene.h"
#include "Brofiler/Brofiler.h"

j1EasingSplines::j1EasingSplines() : j1Module()
{
	name.assign("easingsplines");
	LOG("Construct Spline Module");
}

// Destructor
j1EasingSplines::~j1EasingSplines()
{
	
}

bool j1EasingSplines::PreUpdate()
{
	// check if any loot entity | enemy goes to delete and remove from list
	for (std::list<EaseSplineInfo*>::iterator itr = easing_splines.begin(); itr != easing_splines.end(); )
	{
		j1Entity* link = dynamic_cast<j1Entity*>((*itr)->associatedLoot);
		
		if (link->to_delete || (*itr)->associatedLoot->picked)
		{
			LOG("FUCK");
			itr = easing_splines.erase(itr);
		}
		else
			++itr;
	}

	return true;
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


	std::list<EaseSplineInfov2*>::iterator item2 = easing_splinesv2.begin();

	for (; item2 != easing_splinesv2.end(); ++item2) {
		if (*item2 != nullptr) {
			if (!(*item2)->Update(dt)) {
				delete(*item2);
				(*item2) = nullptr;
			}
		}
	}

	easing_splinesv2.remove(nullptr);
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

	std::list<EaseSplineInfov2*>::iterator item2 = easing_splinesv2.begin();

	for (; item2 != easing_splinesv2.end(); ++item2) {
		if (*item2 != nullptr) {
			delete(*item2);
			(*item2) = nullptr;
		}
	}

	easing_splines.clear();

	LOG("CleanUp Spline Module");

	return true;
}

EaseSplineInfo* j1EasingSplines::CreateSpline(LootEntity* associatedEntity, float* position, const int target_position, const float time_to_travel, TypeSpline type, std::function<void()> fn)
{
	std::list <EaseSplineInfo*>::iterator item = easing_splines.begin();
	for (; item != easing_splines.end(); ++item) {
		if ((*item) != nullptr && (*item)->position == position) {
			(*item)->to_delete = true;
			break;
		}
	}

	EaseSplineInfo* info = DBG_NEW EaseSplineInfo(associatedEntity,position, target_position, time_to_travel, type, fn);

	if (info != nullptr)
		easing_splines.push_back(info);
	else
		LOG("Could not create the Spline...");

	return info;
}

EaseSplineInfov2* j1EasingSplines::CreateSplineV2(float position, int target_position, const float time_to_travel, TypeSpline type,bool axis, std::function<void()> fn)
{
	std::list <EaseSplineInfov2*>::iterator item = easing_splinesv2.begin();
	for (; item != easing_splinesv2.end(); ++item) {
		if ((*item) != nullptr && (*item)->position == position) {
			(*item)->to_delete = true;
			break;
		}
	}

	EaseSplineInfov2* info = DBG_NEW EaseSplineInfov2(position, target_position, time_to_travel, type, axis, fn);

	if (info != nullptr)
		easing_splinesv2.push_back(info);
	else
		LOG("Could not create the Spline...");

	return info;
}





bool EaseSplineInfo::Update(float dt)
{
	BROFILER_CATEGORY("Update splineInfo", Profiler::Color::LawnGreen);
	bool ret = true;

	float time_passed = SDL_GetTicks() - time_started;

	if (time_passed < time_to_travel && !to_delete) {
		switch (type) {
		case EASE: {
			*position = ease_function.Ease(time_passed, initial_position, distance_to_travel, time_to_travel);
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
		} break;
		case EASE_OUT_EXPO: {
			*position = ease_function.EaseOutBounce(time_passed, initial_position, distance_to_travel, time_to_travel);
		} break;
		case EASE_OUT_QUAD: {
			*position = ease_function.EaseOutBounce(time_passed, initial_position, distance_to_travel, time_to_travel);
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

int EaseFunctions::EaseOutExpo(float time_passed, int initial_position, int distance_to_travel, float time_to_travel)
{
	return distance_to_travel  * pow(2, 10 * (time_passed / time_to_travel - 1)) + initial_position;
}

int EaseFunctions::EaseOutQuad(float time_passed, int initial_position, int distance_to_travel, float time_to_travel)
{
	return -distance_to_travel * (time_passed /= time_to_travel) * (time_passed - 2) + initial_position;
}


bool EaseSplineInfov2::Update(float dt)
{
	BROFILER_CATEGORY("Update splineInfo2", Profiler::Color::LawnGreen);
	bool ret = true;

	float time_passed = SDL_GetTicks() - time_started;
	int testCounter = 1;
	if (time_passed < time_to_travel && !to_delete) {
		switch (type) {
			
		case EASE: {
			for (std::list<ConsumableStats>::iterator iter = App->scene->consumableinfo.begin(); iter != App->scene->consumableinfo.end(); ++iter)
			{
				LOG("consum size EUpdt %i", App->scene->consumableinfo.size());
				if ((*iter).counter = testCounter)
				{
					if (axis)
					{
						LOG("ITER SIZE %i", App->scene->consumableinfo.size());
						LOG("test counter %i", App->scene->consumableinfo.size());
						(*iter).position.x = ease_function.Ease(time_passed, initial_position, distance_to_travel, time_to_travel);
						testCounter = 1;
						break;
					}
					else if (!axis)
					{
						(*iter).position.y = ease_function.Ease(time_passed, initial_position, distance_to_travel, time_to_travel);
						testCounter = 1;
						break;;
					}
				}
				++testCounter;

			}
			position = ease_function.Ease(time_passed, initial_position, distance_to_travel, time_to_travel);
			testCounter = 1;
		} break;
		case EASE_OUT_QUINT: {

			for (std::list<ConsumableStats>::iterator iter = App->scene->consumableinfo.begin(); iter != App->scene->consumableinfo.end(); ++iter)
			{
				if (axis)
				{
					(*iter).position.x = ease_function.EaseOutQuint(time_passed, initial_position, distance_to_travel, time_to_travel);
					testCounter = 1;
					break;
				}
				else if (!axis)
				{
					(*iter).position.y = ease_function.EaseOutQuint(time_passed, initial_position, distance_to_travel, time_to_travel);
					testCounter = 1;
					break;
				}

			}
			position = ease_function.EaseOutQuint(time_passed, initial_position, distance_to_travel, time_to_travel);
		} break;
		case EASE_IN_OUT_BACK: {
			for (std::list<ConsumableStats>::iterator iter = App->scene->consumableinfo.begin(); iter != App->scene->consumableinfo.end(); ++iter)
			{
				if ((*iter).counter = testCounter)
				{
					if (axis)
					{
						(*iter).position.x = ease_function.EaseInOutBack(time_passed, initial_position, distance_to_travel, time_to_travel);

						
						testCounter = 1;

						break;
					}
					else if (!axis)
					{
						(*iter).position.y = ease_function.EaseInOutBack(time_passed, initial_position, distance_to_travel, time_to_travel);
						testCounter = 1;
						break;

					}
					
				}

				++testCounter;
			}
			position = ease_function.EaseInOutBack(time_passed, initial_position, distance_to_travel, time_to_travel);
			testCounter = 1;
		} break;
		case EASE_IN_BACK: {
			for (std::list<ConsumableStats>::iterator iter = App->scene->consumableinfo.begin(); iter != App->scene->consumableinfo.end(); ++iter)
			{
				if (axis)
				{
					(*iter).position.x = ease_function.EaseInBack(time_passed, initial_position, distance_to_travel, time_to_travel);

				}
				else if (!axis)
				{
					(*iter).position.y = ease_function.EaseInBack(time_passed, initial_position, distance_to_travel, time_to_travel);
				}

			}
			position = ease_function.EaseInBack(time_passed, initial_position, distance_to_travel, time_to_travel);
		} break;
		case EASE_OUT_BACK: {
			for (std::list<ConsumableStats>::iterator iter = App->scene->consumableinfo.begin(); iter != App->scene->consumableinfo.end(); ++iter)
			{
				if (axis)
				{
					(*iter).position.x = ease_function.EaseOutBack(time_passed, initial_position, distance_to_travel, time_to_travel);

				}
				else if (!axis)
				{
					(*iter).position.y = ease_function.EaseOutBack(time_passed, initial_position, distance_to_travel, time_to_travel);
				}

			}
			position = ease_function.EaseOutBack(time_passed, initial_position, distance_to_travel, time_to_travel);
		} break;
		case EASE_IN_CUBIC: {
			for (std::list<ConsumableStats>::iterator iter = App->scene->consumableinfo.begin(); iter != App->scene->consumableinfo.end(); ++iter)
			{
				if (axis)
				{
					(*iter).position.x = ease_function.EaseInCubic(time_passed, initial_position, distance_to_travel, time_to_travel);

				}
				else if (!axis)
				{
					(*iter).position.y = ease_function.EaseInCubic(time_passed, initial_position, distance_to_travel, time_to_travel);
				}

			}
			position = ease_function.EaseInCubic(time_passed, initial_position, distance_to_travel, time_to_travel);
		} break;
		case EASE_OUT_CUBIC: {

			for (std::list<ConsumableStats>::iterator iter = App->scene->consumableinfo.begin(); iter != App->scene->consumableinfo.end(); ++iter)
			{
				if (axis)
				{
					(*iter).position.x = ease_function.EaseOutCubic(time_passed, initial_position, distance_to_travel, time_to_travel);
				}
				else if (!axis)
				{
					(*iter).position.y = ease_function.EaseOutCubic(time_passed, initial_position, distance_to_travel, time_to_travel);
				}

			}
			position = ease_function.EaseOutCubic(time_passed, initial_position, distance_to_travel, time_to_travel);

		} break;
		case EASE_OUT_BOUNCE: {
			for (std::list<ConsumableStats>::iterator iter = App->scene->consumableinfo.begin(); iter != App->scene->consumableinfo.end(); ++iter)
			{
				if (axis)
				{
					(*iter).position.x = ease_function.EaseOutBounce(time_passed, initial_position, distance_to_travel, time_to_travel);
				}
				else if (!axis)
				{
					(*iter).position.y = ease_function.EaseOutBounce(time_passed, initial_position, distance_to_travel, time_to_travel);
				}

			}
			position = ease_function.EaseOutBounce(time_passed, initial_position, distance_to_travel, time_to_travel);
		} break;
		case EASE_OUT_EXPO: {

			for (std::list<ConsumableStats>::iterator iter = App->scene->consumableinfo.begin(); iter != App->scene->consumableinfo.end(); ++iter)
			{
				if (axis)
				{
					(*iter).position.x = ease_function.EaseOutExpo(time_passed, initial_position, distance_to_travel, time_to_travel);
				}
				else if (!axis)
				{
					(*iter).position.y = ease_function.EaseOutExpo(time_passed, initial_position, distance_to_travel, time_to_travel);
				}

			}
			position = ease_function.EaseOutExpo(time_passed, initial_position, distance_to_travel, time_to_travel);
		} break;
		case EASE_OUT_QUAD: {

			for (std::list<ConsumableStats>::iterator iter = App->scene->consumableinfo.begin(); iter != App->scene->consumableinfo.end(); ++iter)
			{
				if (axis)
				{
					(*iter).position.x = ease_function.EaseOutQuad(time_passed, initial_position, distance_to_travel, time_to_travel);
				}
				else if (!axis)
				{
					(*iter).position.y = ease_function.EaseOutQuad(time_passed, initial_position, distance_to_travel, time_to_travel);
				}

			}
			position = ease_function.EaseOutQuad(time_passed, initial_position, distance_to_travel, time_to_travel);
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
