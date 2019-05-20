#pragma once
#include "Transition.h"
#include "p2Point.h"
#include "j1Scene.h"
#include <vector>

class Squares :
	public Transition
{
private:
	Color color = Black;
	bool is_scene_change = false;
	SceneState scene_to_change = SceneState::MAX_STATES;

	uint width, height, square_width, square_height;
	std::vector<iPoint>positions;

	int total_squares_row_col = 10;

public:
	Squares(float transition_time, bool is_scene_change = false, SceneState scene_to_change = SceneState::STARTMENU, Color color = Black);
	~Squares();

	void Entering();
	void Action();
	void Exiting();

	void SetColor(Color color);
	void DrawSquares(int num_Squares);
};

