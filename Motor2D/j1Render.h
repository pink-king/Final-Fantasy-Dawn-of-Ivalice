#ifndef __j1RENDER_H__
#define __j1RENDER_H__

#include "SDL/include/SDL.h"
#include "p2Point.h"
#include "j1Module.h"

class j1Render : public j1Module
{
public:

	j1Render();

	// Destructor
	virtual ~j1Render();

	// Called before render is available
	bool Awake(pugi::xml_node&);

	// Called before the first frame
	bool Start();

	// Called each loop iteration
	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	// Load / Save
	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&) const;

	// Utils
	void SetViewPort(const SDL_Rect& rect);
	void ResetViewPort();
	iPoint ScreenToWorld(int x, int y) const;
	iPoint WorldToScreen(int x, int y, bool descriptions = false) const;

	bool IsOnCamera(const int&x, const int&y, const int&w, const int& h) const;


	// Draw & Blit
	/*bool Blit(SDL_Texture* texture, int x, int y, const SDL_Rect* section = NULL, float speed = 1.0f, SDL_RendererFlip flip = SDL_FLIP_NONE, float spriteScale = 1.f, double angle = 0, int pivot_x = INT_MAX, int pivot_y = INT_MAX) const;
	bool BlitGui(SDL_Texture* texture, int x, int y, const SDL_Rect* section = NULL, float speed = 1.0f, float scaleFactor = 1.0f, float flippingAngle = 0.0f, SDL_Rect wantedRect = { 0, 0, 0, 0 }) const;*/
	bool Blit(SDL_Texture* texture, int x, int y, const SDL_Rect* section = NULL, float speed = 1.0f, SDL_RendererFlip flip = SDL_FLIP_NONE, float spriteScale = 1.f, double angle = 0, int pivot_x = INT_MAX, int pivot_y = INT_MAX, bool useWindowScale = true) const;
	bool BlitGui(SDL_Texture* texture, int x, int y, const SDL_Rect* section = NULL, float speed = 1.0f, float scaleFactor = 1.0f, float flippingAngle = 0.0f, SDL_Rect wantedRect = { 0, 0, 0, 0 }) const;

	bool DrawQuad(const SDL_Rect& rect, Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255, bool filled = true, bool use_camera = true) const;
	bool DrawIsoQuad(SDL_Rect rect, SDL_Color color = { 255,0,0,255 }) const;
	bool DrawLine(int x1, int y1, int x2, int y2, Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255, bool use_camera = true) const;
	bool DrawCircle(int x1, int y1, int redius, Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255, bool use_camera = true) const;

	// Set background color
	void SetBackgroundColor(SDL_Color color);
	void SetTextureColor(SDL_Texture* tex, uint r, uint g, uint b);
	void SetTextureAlpha(SDL_Texture* tex, uint alpha);
public:

	SDL_Renderer*	renderer = nullptr;
	const SDL_Rect* camera = nullptr;
	SDL_Rect		viewport;
	SDL_Color		background;
};

#endif // __j1RENDER_H__