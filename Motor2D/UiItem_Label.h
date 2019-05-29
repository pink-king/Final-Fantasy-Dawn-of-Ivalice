#ifndef UIITEM_LABEL_H
#define UIITEM_LABEL_H

#include "UiItem.h"
#include <string>
#include "p2Point.h"
#include "SDL_ttf/include/SDL_ttf.h"
#include "j1PerfTimer.h"

class UiItem_Label : public UiItem
{
public:
	UiItem_Label(std::string text, SDL_Color color, TTF_Font * font, p2Point<int> position, UiItem*const parent, bool typewriter = false);
	~UiItem_Label();

	/*UiItem_Label(std::string text, SDL_Color color, TTF_Font * font, bool typewriter, p2Point<int> position, UiItem*const parent);*/
	/*bool ChangeTextureHover(const std::string * textHover, const SDL_Color* color, const TTF_Font* font);
	bool ChangeTextureIdle(const std::string * textHover, const SDL_Color* color, const TTF_Font* font);*/
	bool ChangeTextureIdle(std::string textIdle, const SDL_Color* color, const TTF_Font* font);
	void Draw(const float& dt) override;
	void CleanUp();
	//bool Update(float dt) override;
	bool TypeWriter();


	void DoLogicHovered();
	void DoLogicAbandoned();



protected:
	std::string text;
	std::string typewriter_text;
	//std::string text_aux;
	bool timer_typewriter = false;
	bool spawn_typewriter = false;
	int counter_typewriter = 0;
	bool right_text = false;
	bool left_text = false;
	SDL_Color color;
	TTF_Font* font = nullptr;
	std::string str_potionIg;
	std::string str_phoenixIg;
	int potion_counterIg = 0;
	int phoenix_counterIg = 0;

	j1PerfTimer typewriter_time;
	bool finishedWriting = false; 


public:  
	SDL_Texture* texture = nullptr;
	//bool hide = false;
	friend class j1DialogSystem; 
	
};
#endif
