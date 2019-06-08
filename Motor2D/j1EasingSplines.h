#ifndef __m1EASINGSPLINES_H__
#define __m1EASINGSPLINES_H__

#include "j1Module.h"
#include <list>
#include "SDL/include/SDL_timer.h"
#include <functional>
#include <math.h>
#include <list>
struct SDL_Texture;
struct SDL_Rect;

enum TypeSpline {

	EASE,
	EASE_OUT_QUINT,
	EASE_IN_OUT_BACK,
	EASE_IN_BACK,
	EASE_OUT_BACK,
	EASE_IN_CUBIC,
	EASE_OUT_CUBIC,
	EASE_OUT_BOUNCE,
	EASE_OUT_EXPO,
	EASE_OUT_QUAD,
	NONE_
};

struct EaseFunctions {
	int EaseOutQuint(float time_passed, int initial_position, int distance_to_travel, float time_to_travel);
	int Ease(float time_passed, int initial_position, int distance_to_travel, float time_to_travel);
	int EaseInOutBack(float time_passed, int initial_position, int distance_to_travel, float time_to_travel);
	int EaseInBack(float time_passed, int initial_position, int distance_to_travel, float time_to_travel);
	int EaseOutBack(float time_passed, int initial_position, int distance_to_travel, float time_to_travel);
	int EaseInCubic(float time_passed, int initial_position, int distance_to_travel, float time_to_travel);
	int EaseOutCubic(float time_passed, int initial_position, int distance_to_travel, float time_to_travel);
	int EaseOutBounce(float time_passed, int initial_position, int distance_to_travel, float time_to_travel);
	int EaseOutExpo(float time_passed, int initial_position, int distance_to_travel, float time_to_travel);
	int EaseOutQuad(float time_passed, int initial_position, int distance_to_travel, float time_to_travel);
	//http://www.timotheegroleau.com/Flash/experiments/easing_function_generator.htm
	//https://github.com/jesusgollonet/ofpennereasing/tree/master/PennerEasing
};

struct EaseSplineInfo {
	float* position = nullptr;
	TypeSpline type;
	int initial_position;
	int distance_to_travel;
	float time_to_travel;
	float time_started;
	EaseFunctions ease_function;
	bool to_delete = false;
	bool Update(float dt);

	std::function<void()> fn;

	EaseSplineInfo(float* position, const int target_position, const float time_to_travel, TypeSpline type, std::function<void()> fn) {
		this->position = position;
		this->initial_position = *position;
		this->distance_to_travel = target_position - *position;
		this->type = type;
		this->time_to_travel = time_to_travel;
		this->fn = fn;
		time_started = SDL_GetTicks();
		
	}


	
};

struct EaseSplineInfov2
{
	float position;
	TypeSpline type;
	int initial_position;
	int distance_to_travel;
	float time_to_travel;
	float time_started;
	EaseFunctions ease_function;
	bool to_delete = false;
	bool Update(float dt);
	std::function<void()> fn;
	bool axis;
	EaseSplineInfov2(float position, const int target_position, const float time_to_travel, TypeSpline type, bool axis, std::function<void()> fn)//true = x axis, false = y axis
	{
		this->position = position;
		this->initial_position = position;
		this->distance_to_travel = target_position - position;
		this->type = type;
		this->time_to_travel = time_to_travel;
		this->fn = fn;
		this->axis = axis;
		time_started = SDL_GetTicks();
	}
};
class j1EasingSplines : public j1Module
{
public:

	j1EasingSplines();

	// Destructor
	virtual ~j1EasingSplines();
	
	
	// Called each loop iteration
	bool Update(float dt);

	// Called before quitting
	bool CleanUp();

	EaseSplineInfo* CreateSpline(float* position, int target_position, const float time_to_travel, TypeSpline type, std::function<void()> fn = nullptr);
	EaseSplineInfov2* CreateSplineV2(float position, int target_position, const float time_to_travel, TypeSpline type, bool axis, std::function<void()> fn = nullptr);
private:

	std::list<EaseSplineInfo*> easing_splines;
	std::list<EaseSplineInfov2*> easing_splinesv2;
};

#endif 