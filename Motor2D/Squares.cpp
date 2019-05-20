#include "Squares.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Window.h"
#include "p2Log.h"
#include <algorithm>
#include <random>


Squares::Squares(float transition_time, bool is_scene_change, SceneState scene_to_change, Color color) : Transition(transition_time)
{
	this->is_scene_change = is_scene_change;
	this->scene_to_change = scene_to_change;
	this->color = color;

	App->win->GetWindowSize(width, height);
	SDL_SetRenderDrawBlendMode(App->render->renderer, SDL_BLENDMODE_BLEND);

	square_width = (int)(width / total_squares_row_col);
	square_height = (int)((height / total_squares_row_col)+1);

	for (int x = 0; x < total_squares_row_col; x++)
	{
		for (int y = 0; y < total_squares_row_col; y++)
		{
			positions.push_back({ (int)(x*square_width), (int)(y*square_height) });
		}
	}

	auto rng = std::default_random_engine{};
	std::shuffle(std::begin(positions), std::end(positions), rng);

}

Squares::~Squares()
{
}

void Squares::Entering()
{
	Transition::Entering();

	int num_Bars = (int)LerpValue(percent, 0, total_squares_row_col * total_squares_row_col);

	DrawSquares(num_Bars);
}

void Squares::Action()
{
	DrawSquares(total_squares_row_col * total_squares_row_col);

	if (is_scene_change)
	{
		App->scene->LoadScene(scene_to_change);
	}

	Transition::Action();
}

void Squares::Exiting()
{
	Transition::Exiting();

	int num_Bars = (int)LerpValue(percent, total_squares_row_col * total_squares_row_col, 0);

	DrawSquares(num_Bars);
}

void Squares::SetColor(Color color)
{
	this->color = color;
}

void Squares::DrawSquares(int num_Squares)
{
	SDL_Rect* Squares = new SDL_Rect[num_Squares];

	for (int i = 0; i < num_Squares; i++)
	{
		Squares[i].x = positions[i].x;
		Squares[i].y = positions[i].y;
		Squares[i].w = square_width;
		Squares[i].h = square_height;
	}

	SDL_SetRenderDrawColor(App->render->renderer, color.r, color.g, color.b, 255);
	SDL_RenderFillRects(App->render->renderer, Squares, num_Squares);
}